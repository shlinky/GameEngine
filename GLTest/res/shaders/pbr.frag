#version 330 core

layout(location = 0) out vec4 color;
in vec4 pos_raw;
in vec3 normals_raw;
vec3 ngoodr;
in vec2 UV;
in vec3 tangents_raw;
vec3 l;
vec3 c;
float la = 2;
uniform mat4 mvp;
uniform vec3 light_color;
uniform vec3 light_position;
uniform vec3 camera_position;
float PI = 3.14159265;

layout(binding=0)uniform sampler2D colorTex;
layout(binding=1)uniform sampler2D normalTex;
layout(binding=2)uniform sampler2D ORM;
layout(binding=3)uniform samplerCube skybox;

vec3 display_world_vector(vec3 v) {
	return vec3((v[0] + 1) / 2, (v[1] + 1) / 2, (v[2] + 1) / 2);
}

float DistributionGGX(vec3 N, vec3 H, float roughness)
{
    float a      = roughness*roughness;
    float a2     = a*a;
    float NdotH  = max(dot(N, H), 0.0);
    float NdotH2 = NdotH*NdotH;
	
    float num   = a2;
    float denom = (NdotH2 * (a2 - 1.0) + 1.0);
    denom = PI * denom * denom;
	
    return num / denom;
}

float GeometrySchlickGGX(float NdotV, float roughness)
{
    float r = (roughness + 1.0);
    float k = (r*r) / 8.0;

    float num   = NdotV;
    float denom = NdotV * (1.0 - k) + k;
	
    return num / denom;
}
float GeometrySmith(vec3 N, vec3 V, vec3 L, float roughness)
{
    float NdotV = max(dot(N, V), 0.0);
    float NdotL = max(dot(N, L), 0.0);
    float ggx2  = GeometrySchlickGGX(NdotV, roughness);
    float ggx1  = GeometrySchlickGGX(NdotL, roughness);
	
    return ggx1 * ggx2;
}
vec3 fresnelSchlick(float cosTheta, vec3 F0)
{
    return F0 + ((1.0 - F0) * pow(1.0 - cosTheta, 5.0));
}  

void main() {

	ngoodr = normalize(normals_raw);
	vec3 tangents_raww = normalize(tangents_raw);
	vec3 bitangents_raw = (cross(ngoodr, tangents_raww));
	tangents_raww = (cross(ngoodr, bitangents_raw));

	vec3 nnorm = vec3(texture(normalTex, UV)) * 2 - 1;
	vec3 normals_final = normalize(nnorm.x * tangents_raww + nnorm.y * bitangents_raw + nnorm.z * ngoodr);
	//normals_final = ngoodr;
	vec3 l = normalize(vec3(0, 0.2, 1));
	float distance = 1;
	vec3 radiance = vec3(1, 1, 1) * 1;
	vec3 c = normalize(camera_position - pos_raw.xyz);
	vec3 h = normalize(c + l);
	float rough = vec3(texture(ORM, UV)).y;
	float metal = vec3(texture(ORM, UV)).z;
	vec3 base_color = vec3(texture(colorTex, UV));

	vec3 F0 = vec3(0.03); 
	F0 = mix(F0, base_color, metal);
	vec3 f = fresnelSchlick(max(dot(h, c), 0.0), F0);

	vec3 DFG = DistributionGGX(normals_final, h, rough) * clamp(GeometrySmith(normals_final, c, l, rough), 0, 1) * f * vec3(1);
	vec3 spec = DFG / max((4 * clamp(dot(c, normals_final), 0, 1) * clamp(dot(l, normals_final), 0, 1)), 0.005);
	vec3 diffuse = (vec3(1.0) - f) * (1.0 - metal) * base_color / PI;
	vec3 BRDF = spec + diffuse;

	vec3 lightout = BRDF * radiance * clamp(dot(l, normals_final), 0, 1);
	vec3 ambient = vec3(0.1) * base_color * vec3(texture(ORM, UV)).x;

	lightout = lightout + ambient;

	color = vec4(lightout, 1);
}    