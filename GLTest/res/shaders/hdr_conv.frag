#version 330 core
layout(location = 0) out vec4 color;
in vec4 pos_raw;
in vec3 normals_raw;
uniform sampler2D equirectangularMap;
uniform vec3 camera_position;

const vec2 invAtan = vec2(0.1591, 0.3183);
vec2 SampleSphericalMap(vec3 v)
{
    vec2 uv = vec2(atan(v.z, v.x), asin(v.y));
    uv *= invAtan;
    uv += 0.5;
    return uv;
}

vec3 display_world_vector(vec3 v) {
	return vec3((v[0] + 1) / 2, (v[1] + 1) / 2, (v[2] + 1) / 2);
}

void main()
{		
    vec2 uv = SampleSphericalMap(normalize(pos_raw.xyz)); // make sure to normalize localPos
    vec3 colorr = texture(equirectangularMap, uv).rgb;
    
    //display_world_vector(normalize(pos_raw.xyz))
    color = vec4(colorr, 1.0);
}