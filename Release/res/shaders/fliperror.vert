#version 330 core

layout(location = 0) in vec3 position;
out vec3 UV;
uniform vec3 face;

void main() {
	gl_Position = vec4(position, 1);                                                               
	if (face == 5)
		UV = vec3(position.xy, -1);
	if (face == 6)
		UV = vec3(position.xy, 1);
}


