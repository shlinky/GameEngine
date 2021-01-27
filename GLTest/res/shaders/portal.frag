#version 330 core

layout(location = 0) out vec4 color;
in vec4 pos_raw;
in vec3 normals_raw;
in vec2 UV;
vec3 l;
vec3 c;
uniform mat4 coolbeans;
uniform vec3 light_color;
uniform vec3 light_position;
uniform vec3 camera_position;

uniform sampler2D ourTexture;

vec3 display_world_vector(vec3 v) {
	return vec3((v[0] + 1) / 2, (v[1] + 1) / 2, (v[2] + 1) / 2);
}

void main() {
	vec3 base_color = vec3(0.2, 0.8, 0.7);

	// vec3 tangents_raww = normalize(tangents_raw);
	// vec3 bitangents_raw = cross(normals_raw, tangents_raww);
	// tangents_raww = cross(normals_raw, bitangents_raw);

	// vec3 nnorm = vec3(texture(normalTex, UV)) * 2 - 1;
	// vec3 normals_final = nnorm.x * tangents_raww + nnorm.y * bitangents_raw + nnorm.z * normals_raw;

	l = vec3(1, 1, 0);
	float distance = length(l);
	float light_factor = clamp(dot(normalize(l), normals_raw), 0, 1);
	float light_strength = 2;
	float ambient_lighting = 0.02;
	float diffuse_lighting = (light_factor * light_strength / (distance * distance));

	c = normalize(camera_position - pos_raw.xyz);
	vec3 reflection = reflect(-normalize(l), normals_raw);
	float spec = pow(clamp(dot(c, reflection), 0, 1), 8);
	float specular_lighting = 1.0 * spec / (distance); 

	vec3 lighting = (diffuse_lighting + ambient_lighting) * vec3(1, 1, 1);

	//display_world_vector(normals_raw)
	//lighting * base_color + (specular_lighting * light_color)
	//vec3 base_color = display_world_vector(pos_raw.xyz);
	color = vec4(lighting * base_color + (specular_lighting * light_color), 1);
}
