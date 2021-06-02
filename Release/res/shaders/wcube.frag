#version 330 core

layout(location = 0) out vec4 color;
in vec4 pos_raw;
in vec3 normals_raw;
in vec2 UV;
vec3 l;
vec3 c;
float la = 1.6;
uniform samplerCube skybox;

vec3 display_world_vector(vec3 v) {
	return vec3((v[0] + 1) / 2, (v[1] + 1) / 2, (v[2] + 1) / 2);
}

void main() {
	// /texture(skybox, vec3(normalize(pos_raw)))
	vec3 hdrColor = texture(skybox, vec3(pos_raw)).rgb;

	vec3 mapped = hdrColor / (hdrColor + vec3(1.0));
    // gamma correction 
    mapped = pow(mapped, vec3(1.0 / 2.2));

    color = vec4(mapped, 1);

}
