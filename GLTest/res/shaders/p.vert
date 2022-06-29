#version 330 core

layout(location = 0) in vec3 position;
out vec2 UV;
uniform mat4 mvp;

void main() {
	gl_Position = mvp * vec4(position, 1);
	vec4 newpos = mvp * vec4(position, 1);
	UV = vec2(position.x, position.y / 2);
	//UV = (UV.xy * 0.9 + 1) * 0.5;
}


