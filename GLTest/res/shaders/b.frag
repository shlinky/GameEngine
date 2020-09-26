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
	//color = vec4((pos_raw[0] + 1) / 2, (pos_raw[1] + 1) / 2, (pos_raw[2] + 1) / 2, 1);
	//vec3 base_color = vec3((normals_raw[0] + 1) / 2, (normals_raw[1] + 1) / 2, (normals_raw[2] + 1) / 2);

	vec3 base_color = vec3(0.2, 0.8, 0.7);

	l = light_position - pos_raw.xyz;
	float distance = length(l);
	float light_factor = clamp(dot(normalize(l), normals_raw), 0, 1);
	float light_strength = 2;
	float ambient_lighting = 0.02;
	float diffuse_lighting = (light_factor * light_strength / (distance * distance));

	//vec3 reflection = normalize(2 * light_factor * normals_raw - normalize(l));
	c = normalize(camera_position - pos_raw.xyz);
	vec3 reflection = reflect(-normalize(l), normals_raw);
	float spec = pow(clamp(dot(c, reflection), 0, 1), 16);
	float specular_lighting = 0.5 * spec / (distance); 

	vec3 lighting = (ambient_lighting + diffuse_lighting) * light_color;

	//display_world_vector(normals_raw)
	//lighting * base_color + (specular_lighting * light_color)
	color = vec4(display_world_vector(normals_raw), 1);
	//color = vec4(0.5, 0, 1, 1);
}
