#version 330 core

layout(location = 0) out vec4 color;
in vec4 gl_FragCoord;
layout(binding=0)uniform sampler2D t1;



vec3 display_world_vector(vec3 v) {
	return vec3((v[0] + 1) / 2, (v[1] + 1) / 2, (v[2] + 1) / 2);
}


void main() {

	//color = vec4(vec3(texture(t1, UV)), 1);
	vec2 uv = vec2(gl_FragCoord.x / 1920, gl_FragCoord.y / 1080);
	vec3 bc = vec3(texture(t1, uv));
	
	color = vec4(bc, 1);
}    
