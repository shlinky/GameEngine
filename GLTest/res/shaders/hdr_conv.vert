#version 330 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normals;
layout(location = 2) in vec2 uv;
layout(location = 3) in vec3 tangents;
out vec4 pos_raw;
out vec3 normals_raw;
uniform mat4 mvp;
uniform mat4 world;
uniform vec3 camera_position;

void main() {
	gl_Position = mvp * vec4(position, 1);                                                               

	pos_raw = world * vec4(position, 1);
	normals_raw = normals;
}

