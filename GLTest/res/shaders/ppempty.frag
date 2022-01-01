#version 330 core
#extension GL_ARB_shading_language_420pack: enable

layout(location = 0) out vec4 color;
in vec2 UV;
layout(binding=0)uniform sampler2D t1;

uniform vec3 selected;
uniform float HDR;
const float offset = 1.0 / 1000.0; 

vec2 offsets[9] = vec2[](
        vec2(-offset,  offset), // top-left
        vec2( 0.0f,    offset), // top-center
        vec2( offset,  offset), // top-right
        vec2(-offset,  0.0f),   // center-left
        vec2(0.0f,  0.0f),   // center-left
        vec2( offset,  0.0f),   // center-right
        vec2(-offset, -offset), // bottom-left
        vec2( 0.0f,   -offset), // bottom-center
        vec2( offset, -offset)  // bottom-right    
    );

float kernel[9] = float[](
		0.11, 0.11, 0.11,
		0.11, 0.11, 0.11,
		0.11, 0.11, 0.11
	);

vec3 display_world_vector(vec3 v) {
	return vec3((v[0] + 1) / 2, (v[1] + 1) / 2, (v[2] + 1) / 2);
}

float grey(vec3 v) {
	return (v.x * 0.33 + v.y * 0.33 + v.z * 0.33);
}

void main() {
	vec3 pixel = vec3(texture(t1, UV));
	float blur_amount = grey(pixel) / (grey(pixel) + 1.25);

	vec3 b;

	for (int i = 0; i < 9; i++) {
		b += vec3(texture(t1, UV + offsets[i])) * kernel[i];
	}

	vec3 lightout = mix(pixel, b, blur_amount);
	if (blur_amount < 0.5) {
		lightout = pixel;
	}
	if (blur_amount > 0.9) {
		lightout = b;
	}
	lightout = pixel;
	vec3 mapped;
	if (HDR < 0.5) {
		mapped = lightout / (lightout + vec3(1.0));
    	mapped = pow(mapped, vec3(1.0 / 2.2));
	}
	else {
		mapped = lightout;
	}
	color = vec4(mapped, 1);
}    
