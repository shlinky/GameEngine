#version 330 core
#extension GL_ARB_shading_language_420pack: enable

layout(location = 0) out vec4 color;
in vec2 UV;
layout(binding=0)uniform sampler2D t1;
layout(binding=1)uniform sampler2D t2;

uniform vec3 selected;
const float offset = 1.0 / 1000.0; 

vec2 offsets[8] = vec2[](
        vec2(-offset,  offset), // top-left
        vec2( 0.0f,    offset), // top-center
        vec2( offset,  offset), // top-right
        vec2(-offset,  0.0f),   // center-left
        vec2( offset,  0.0f),   // center-right
        vec2(-offset, -offset), // bottom-left
        vec2( 0.0f,   -offset), // bottom-center
        vec2( offset, -offset)  // bottom-right    
    );

float kernel[9] = float[](
		-1, -1, -1,
		-1, 9, -1,
		-1, -1, -1
	);

float kernelo[9] = float[](
		-1, -1, -1,
		-1, 8, -1,
		-1, -1, -1
	);

vec3 display_world_vector(vec3 v) {
	return vec3((v[0] + 1) / 2, (v[1] + 1) / 2, (v[2] + 1) / 2);
}

float grey(vec3 v) {
	return (v.x * 0.2126 + v.y * 0.7152 + v.z * 0.0722);
}

void main() {

	//color = vec4(vec3(texture(t1, UV)), 1);
	vec3 bc = vec3(texture(t1, UV));
	

	vec3 final = bc;
	for (int i = 0; i < 8; i++) {
		if (length(vec3(texture(t2, UV + offsets[i])) - vec3(texture(t2, UV))) >= 0.001) {
			final = vec3(1, 1, 1);
		}
	}

	color = vec4(final, 1);
	//color = vec4(UV, 0, 1);
}    
