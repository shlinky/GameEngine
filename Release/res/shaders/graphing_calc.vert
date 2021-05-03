#version 330 core

layout(location = 0) in vec3 position;
out vec4 pos_raw;
uniform mat4 coolbeans;
uniform mat4 world;

void main() {
	gl_Position = coolbeans * vec4(position, 1);                                                               

	pos_raw = world * vec4(position, 1);
}


