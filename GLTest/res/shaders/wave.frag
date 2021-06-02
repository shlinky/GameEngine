
// Normalized pixel coordinates (from 0 to 1)
vec2 uv = fragCoord/iResolution.xy;

float d = pow((0.5 - uv.x)*(0.5 - uv.x) + (0.5 - uv.y)*(0.5 - uv.y), 0.5) * 100.0;
float z = 1.0;

//float horiz = -1.0 * (sin(d) - d*cos(d)) / (d*d);
float horiz = -1.0 * (sin(d) + log(1.01)*d*cos(d)) / pow(1.01, (d*d));
vec2 xy = normalize(vec2(uv.x - 0.5, uv.y - 0.5)) * horiz;

fragColor = vec4(normalize(vec3(xy.x, xy.y, 1.0) * 2.0 + 1.0), 1);
    