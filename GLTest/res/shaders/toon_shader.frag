#version 330 core

layout(location = 0) out vec4 color;
in vec4 pos_raw;
in vec3 normals_raw;
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

uniform samplerCube skybox;
layout(binding=0)uniform sampler2D colorTex;
layout(binding=1)uniform sampler2D normalTex;
layout(binding=2)uniform sampler2D ORM;

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
    return F0 + (1.0 - F0) * pow(1.0 - cosTheta, 5.0);
}  

void main() {
	// vec3 normals_final = normals_raw;

	// // vec3 l = normalize(light_position - pos_raw.xyz);
	// // float distance = length(light_position - pos_raw.xyz);
	// vec3 l = vec3(-1, 0.5, 0);
	// float distance = 1;
	// vec3 radiance = light_color / (distance * distance);
	// vec3 c = normalize(camera_position - pos_raw.xyz);
	// vec3 h = normalize(c + l);
	// float rough = 0.3;
	// float metal = 0.5;
	//vec3 base_color = vec3(1, 0.8567, 0);

	// vec3 F0 = vec3(0.04); 
	// F0 = mix(F0, base_color, metal);
	// vec3 f = fresnelSchlick(max(dot(h, c), 0.0), F0);

	// vec3 DFG = DistributionGGX(normals_final, h, rough) * GeometrySmith(normals_final, c, l, rough) * f;
	// vec3 spec = DFG / max((4 * clamp(dot(c, normals_final), 0, 1) * clamp(dot(l, normals_final), 0, 1)), 0.005);
	// vec3 diffuse = (vec3(1.0) - f) * (1.0 - metal) * base_color / PI;
	// //vec3 BRDF = clamp(spec, vec3(0), vec3(1)) + diffuse;
	// vec3 BRDF = spec + diffuse;

	// vec3 lightout = BRDF * radiance * clamp(dot(l, normals_final), 0, 1);
	// vec3 ambient = vec3(0.03) * base_color * 1;

	// lightout = lightout;

	// color = vec4(lightout, 1);

	vec3 tangents_raww = normalize(tangents_raw);
	vec3 bitangents_raw = cross(normals_raw, tangents_raww);
	tangents_raww = cross(normals_raw, bitangents_raw);

	vec3 nnorm = vec3(texture(normalTex, UV)) * 2 - 1;
	vec3 normals_final = nnorm.x * tangents_raww + nnorm.y * bitangents_raw + nnorm.z * normals_raw;
	//normals_final = normals_raw;

	vec3 l = normalize(vec3(1, 2, 4) - pos_raw.xyz);
	float distance = 1;
	vec3 radiance = vec3(1, 1, 1);
	vec3 c = normalize(camera_position - pos_raw.xyz);
	// vec3 h = normalize(c + l);
	// float rough = vec3(texture(ORM, UV)).y;
	// float metal = vec3(texture(ORM, UV)).z;
	vec3 base_color = vec3(texture(colorTex, UV));

	// vec3 F0 = vec3(0.08); 
	// F0 = mix(F0, base_color, metal);
	// vec3 f = fresnelSchlick(max(dot(h, c), 0.0), F0);

	// vec3 DFG = DistributionGGX(normals_final, h, rough) * GeometrySmith(normals_final, c, l, rough) * f * vec3(1);
	// vec3 spec = DFG / max((4 * clamp(dot(c, normals_final), 0, 1) * clamp(dot(l, normals_final), 0, 1)), 0.005);
	// vec3 diffuse = (vec3(1.0) - f) * (1.0 - metal) * base_color / PI;
	// //vec3 BRDF = clamp(spec, vec3(0), vec3(1)) + diffuse;
	// vec3 BRDF = spec + diffuse;

	// vec3 lightout = BRDF * radiance * clamp(dot(l, normals_final), 0, 1);
	// vec3 ambient = vec3(0.03) * base_color * vec3(texture(ORM, UV)).x;

	// lightout = lightout + ambient;

	float diffuse = dot(normals_raw, l);
	if (diffuse > 0.2) {
		diffuse = 1;
	}
	else {
		diffuse = 0.2;
	}
	vec3 r = reflect(-l, normals_raw);

	float spec = pow(clamp(dot(c, r), 0, 1), 80);

	if (spec > 0.3) {
		spec = 0.5;
	}
	else {
		spec = 0;
	}
	vec3 s = spec * vec3(0.2, 0.2, 0.3);

	//vec4(ndoth * vec3(1), 1);
	//diffuse * base_color + s
	color = vec4(diffuse * base_color + s, 1);
}    