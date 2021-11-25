#version 330 core
#extension GL_ARB_shading_language_420pack: enable

layout(location = 0) out vec4 color;
layout(location = 2) out vec4 icolor;
uniform vec3 col;

void main() {
	color = vec4(col, 1);
	float id = 1;
	if (col.y > 0) {
		id += 1;
	}
	else if (col.z > 0) {
		id += 2;
	}
	icolor = vec4(id / 255, id / 255, id / 255, 1);
}    