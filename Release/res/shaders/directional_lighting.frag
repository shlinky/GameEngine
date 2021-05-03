#version 330 core

layout(location = 0) out vec4 color;
in vec4 pos_raw;
in vec3 normals_raw;
in vec2 UV;
vec3 l;
vec3 c;
float la = 1.6;
uniform mat4 coolbeans;
uniform vec3 light_color;
uniform vec3 light_position;
uniform vec3 camera_position;

uniform samplerCube skybox;
layout(binding=1)uniform sampler2D colorTex;

vec3 display_world_vector(vec3 v) {
	return vec3((v[0] + 1) / 2, (v[1] + 1) / 2, (v[2] + 1) / 2);
}

void main() {
	vec3 base_color = vec3(0.2, 0.8, 0.7);

	l = vec3(-cos(la), sin(la), 0);
	float light_factor = clamp(dot(normalize(l), normals_raw), 0, 1);
	float light_strength = 2;
	float ambient_lighting = 0.2;
	float diffuse_lighting = (light_factor * light_strength);

	c = normalize(camera_position - pos_raw.xyz);
	vec3 reflection = reflect(-normalize(c), normals_raw);
	float spec = dot(c, reflection);
	float specular_lighting = 1.0 * spec; 

	vec3 lighting = (diffuse_lighting + ambient_lighting) * light_color;

	//display_world_vector(normals_raw)
	//lighting * base_color + (specular_lighting * light_color)
	//vec3 base_color = display_world_vector(pos_raw.xyz);
	//vec3(texture(skybox, reflection)) * vec3(texture(colorTex, UV))
	//display_world_vector(vec3(UV, 1))
	//0.5 * vec3(texture(skybox, reflection)) + vec3(texture(skybox, reflection)) * vec3(texture(colorTex, UV))
	color = vec4(display_world_vector(normalize(normals_raw)), 1);
}    
