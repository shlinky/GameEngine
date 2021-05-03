#version 330 core

layout(location = 0) out vec4 color;
in vec2 UV;

float PI = 3.141593;

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
	float r = 0.01;
	float ndotv = UV.y;

	vec3 v = vec3(sqrt(1 - pow(ndotv, 2)), ndotv, 0);
	vec2 integral = vec2(0, 0);

    vec3 test = vec3(0, 0, 0);
	float sampleDelta = 0.025;
    float nrSamples = 0.0; 
    for(float b = 0.0; b < 2.0 * PI; b += sampleDelta)
    {
        for(float a = 0.0; a < 0.5 * PI; a += sampleDelta)
        {
        	vec3 l = normalize(vec3(cos(a) * sin(b), sin(a), cos(a) * cos(b)));
        	vec3 h = normalize(v + l);
        	vec3 n = vec3(0, 1, 0);
        	float e = DistributionGGX(n, h, r) * GeometrySmith(n, v, l, r);
            e /= max((4 * clamp(dot(v, n), 0, 1) * clamp(dot(l, n), 0, 1)), 0.005);
        	float e1 = e * (1 - pow(1 - dot(v, h), 5)) * sin(a);
        	float e2 = e * pow(1 - dot(v, h), 5) * sin(a);
        	test += h * sin(a);
            integral += vec2(e2, e2);
   			nrSamples += 1;
        }
    }
    integral /= 1;
    test /= nrSamples;
    color = vec4(integral, 0, 1);

}
