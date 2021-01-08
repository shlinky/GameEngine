#version 330 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec2 uv;
layout(location = 2) in float img;
out vec4 pos_raw;
uniform mat4 coolbeans;
out vec2 UV;
out float imag;

void main() {
	gl_Position = coolbeans * vec4(position, 1);                                                               

	pos_raw = vec4(position, 1);

	UV = uv;
	imag = img;
}


