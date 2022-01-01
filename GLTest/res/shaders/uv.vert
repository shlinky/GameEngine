#version 330 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normals;
layout(location = 2) in vec2 uv;
layout(location = 3) in vec3 tangents;
out vec4 pos_raw;
out vec3 normals_raw;
out vec2 UV;
out vec3 tangents_raw;
uniform mat4 mvp;
uniform mat4 world;
uniform vec3 light_position;
uniform vec3 light_color;
uniform vec3 camera_position;

void main() {
	gl_Position = mvp * vec4(position, 1);                                                               

	pos_raw = world * vec4(position, 1);
	normals_raw = vec3(world * vec4(normals, 0));

	UV = uv;
	tangents_raw = vec3(world * vec4(tangents, 0));
}



