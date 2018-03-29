#pragma include "common/frag.glsl"

uniform sampler2DRect last;

uniform float weight;

vec3 hsvMix(vec3 present, vec3 past)
{
    vec3 future = mix(rgb2hsv(present), rgb2hsv(past), weight);

    return hsv2rgb(future);
}

vec3 rgbMix(vec3 present, vec3 past)
{
    return mix(present, past, weight);
}

void main()
{
    vec4 present = texture(tex0, texture_coord);
    vec4 past = texture(last, texture_coord);
    vec3 future = rgbMix(present.rgb, past.rgb);

    outputColor = vec4(future, present.w);
}