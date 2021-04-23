#version 330 core
layout(location = 0) out vec4 color;
in vec4 pos_raw;
in vec3 normals_raw;
uniform samplerCube cm;
uniform vec3 camera_position;
float PI = 3.14159265;
int STEPS = 100;
vec3 display_world_vector(vec3 v) {
	return vec3((v[0] + 1) / 2, (v[1] + 1) / 2, (v[2] + 1) / 2);
}

void main()
{		
    vec3 uv = normalize(pos_raw.xyz);
    vec3 colorr = texture(cm, uv).rgb;

 //    float stepa = PI/(2 * STEPS);
 //    float stepb = PI * (2 / STEPS);

 //    vec3 up = uv;
 //    vec3 right = vec3(1.0, 0.0, 0.0);
	// vec3 forward = normalize(cross(up, right));
	// right = normalize(cross(up, forward));

 //    vec3 integral = colorr;
 //    for (int a = 0; a < STEPS; a++) {
 //    	for (int b = 0; b < STEPS; b++) {
 //    		float t1 = stepa * a;
 //    		float t2 = stepb * b;
 //    		vec3 samplev = normalize(vec3(cos(t1) * sin(t2), sin(t1), cos(t1) * cos(t2)));

 //    		samplev = samplev.x * right + samplev.y * up + samplev.z * forward;
 //    		//display_world_vector(samplev)
 //    		integral += texture(cm, samplev).rgb  * cos(t1) * sin(t1);
 //    	}
 //    }
 //    integral = integral / (STEPS * STEPS);

 vec3 irradiance = vec3(0.0);  

    vec3 up    = vec3(0.0, 1.0, 0.0);
    vec3 right = normalize(cross(up, uv));
    up         = normalize(cross(uv, right));

    float sampleDelta = 0.025;
    float nrSamples = 0.0; 
    for(float b = 0.0; b < 2.0 * PI; b += sampleDelta)
    {
        for(float a = 0.0; a < 0.5 * PI; a += sampleDelta)
        {
            // spherical to cartesian (in tangent space)
            vec3 tangentSample = vec3(sin(a) * cos(b),  sin(a) * sin(b), cos(a));
            // tangent space to world
            vec3 sampleVec = tangentSample.x * right + tangentSample.y * up + tangentSample.z * uv; 

            irradiance += texture(cm, sampleVec).rgb * cos(a) * sin(a);
            nrSamples++;
        }
    }
    irradiance = PI * irradiance * (1.0 / float(nrSamples));
    color = vec4(irradiance * 3, 1.0);
}