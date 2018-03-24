#version 330

uniform sampler2DRect tex0;
uniform sampler2DRect last;

uniform float time;
uniform float weight;

uniform vec2 resolution;

out vec4 outputColor;

in vec2 texture_coord;

vec3 rgb2hsv(vec3 c)
{
    vec4 K = vec4(0.0, -1.0 / 3.0, 2.0 / 3.0, -1.0);
    vec4 p = mix(vec4(c.bg, K.wz), vec4(c.gb, K.xy), step(c.b, c.g));
    vec4 q = mix(vec4(p.xyw, c.r), vec4(c.r, p.yzx), step(p.x, c.r));

    float d = q.x - min(q.w, q.y);
    float e = 1.0e-10;

    return vec3(abs(q.z + (q.w - q.y) / (6.0 * d + e)), d / (q.x + e), q.x);
}

vec3 hsv2rgb(vec3 c)
{
    vec4 K = vec4(1.0, 2.0 / 3.0, 1.0 / 3.0, 3.0);
    vec3 p = abs(fract(c.xxx + K.xyz) * 6.0 - K.www);

    return c.z * mix(K.xxx, clamp(p - K.xxx, 0.0, 1.0), c.y);
}

vec3 hsvMix(vec3 present, vec3 past)
{
    vec3 future = mix(rgb2hsv(present), rgb2hsv(past), weight);

    return hsv2rgb(future);
}

vec3 rgbMix(vec3 present, vec3 past)
{
    return mix(present, past, weight);
}

vec3 rgbAverage(vec3 present, vec3 past)
{
    return ((present * (1 - weight)) + (past * (1 + weight))) / 2.0;
}

void main()
{
    vec4 present = texture(tex0, texture_coord);
    vec4 past = texture(last, texture_coord);
    vec3 future = rgbAverage(present.rgb, past.rgb);

    outputColor = vec4(future, present.w);
}