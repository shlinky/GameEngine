#version 330 core
#extension GL_ARB_shading_language_420pack: enable

layout(location = 0) out vec4 color;
in vec2 UV;
uniform sampler2D t1;
const float offset = 1.0 / 500.0; 

vec2 offsets[9] = vec2[](
        vec2(-offset,  offset), // top-left
        vec2( 0.0f,    offset), // top-center
        vec2( offset,  offset), // top-right
        vec2(-offset,  0.0f),   // center-left
        vec2( 0.0f,    0.0f),   // center-center
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
	bc = ((bc.x * 0.2126 + bc.y * 0.7152 + bc.z * 0.0722) + 0.01) * vec3(1, 1, 1);
	// vec3 final = vec3(0, 0, 0);
	// for (int i = 0; i < 9; i++) {
	// 	final += vec3(texture(t1, UV + offsets[i])) * kernel[i];
	// }
	// float outline = 0;
	// for (int i = 0; i < 9; i++) {
	// 	outline += (1 - vec3(texture(t2, UV + offsets[i])).x) * kernelo[i];
	// }
	// //final /= 9;

	// //(1 - vec3(texture(depth, UV)).x)
	// outline = pow((1 - outline), 1024);
	// vec3 o;
	//  if (outline < 0.5) {
	//  	o = vec3(1, 1, 1);
	//  }
	//  else {
	//  	o = vec3(0, 0, 0);
	//  }
	//+ o * vec3(1, 1, 1
	color = vec4(bc, 1);
	//color = vec4(UV, 0, 1);
}    
