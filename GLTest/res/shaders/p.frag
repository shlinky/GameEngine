#version 330 core

layout(location = 0) out vec4 color;
in vec4 gl_FragCoord;
in vec2 UV;
layout(binding=0)uniform sampler2D t1;
layout(binding=1)uniform sampler2D t2;


vec3 display_world_vector(vec3 v) {
	return vec3((v[0] + 1) / 2, (v[1] + 1) / 2, (v[2] + 1) / 2);
}


void main() {

	//color = vec4(vec3(texture(t1, UV)), 1);
	vec2 uv = vec2(gl_FragCoord.x / 1920, gl_FragCoord.y / 1080);
	vec3 f = vec3(texture(t2, UV));
	vec3 bc = vec3(texture(t1, uv));
	
	if ((abs(UV.x) > 0.95) || (abs(UV.y) > 0.95)) {
		color = vec4(0, 100, 0, 1);
	}
	else {
		color = vec4(bc, 1);
	}
}    

