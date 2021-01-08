#version 330 core

layout(location = 0) out vec4 color;
in vec4 pos_raw;
in vec2 UV;
in float imag;
layout(binding=0)uniform sampler2D t1;
layout(binding=1)uniform sampler2D t2;


vec3 display_world_vector(vec3 v) {
	return vec3((v[0] + 1) / 2, (v[1] + 1) / 2, (v[2] + 1) / 2);
}

void main() {

	//color = vec4(vec3(texture(t1, UV)), 1);
	vec3 bc = vec3(texture(t1, UV));
	float avg = (bc.x + bc.y + bc.z) / 3;
	if (avg > 0.9) {
		discard;
	}
	color = vec4(vec3(texture(t1, UV)), 1);
}    
