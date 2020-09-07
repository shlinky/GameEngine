#version 330 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normals;
layout(location = 2) in vec2 uv;
out vec4 pos_raw;
out vec3 normals_raw;
out vec3 l;
out vec2 UV;
uniform mat4 coolbeans;
uniform vec3 light_position;
uniform vec3 light_color;
uniform vec3 camera_position;

void main() {
	gl_Position = coolbeans * vec4(position, 1);                                                               

	vec3 light_direction = light_position - position;	
	l = light_direction;

	pos_raw = vec4(position, 1);
	normals_raw = normals;

	UV = uv;
}
