#version 330 core

layout(location = 0) out vec4 color;
in vec4 pos_raw;
uniform mat4 coolbeans;
uniform mat4 world;

vec3 display_world_vector(vec3 v) {
	return vec3((v[0] + 1) / 2, (v[1] + 1) / 2, (v[2] + 1) / 2);
}

void main() {
	color = vec4(display_world_vector(pos_raw.xyz), 1);
}


