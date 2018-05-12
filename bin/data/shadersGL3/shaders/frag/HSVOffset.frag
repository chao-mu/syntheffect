#pragma include "include/header.glsl"

uniform float hueOffset = 0.;
uniform float saturationOffset = 0.;
uniform float valueOffset = 0.;

#define EPSILON 1.0e-10

float mod01(float v) {
    if (v + EPSILON >= 1. && v - EPSILON <= 1) {
        return 1.;
    }

    return mod(v, 1.);
}

vec3 mainFrag()
{
    vec4 color = texture(tex0, textureCoordinate);
    
    vec3 hsv = rgb2hsv(color.rgb);

    hsv.x = mod01(hsv.x + hueOffset);
    hsv.y = mod01(hsv.y + saturationOffset);
    hsv.z = mod01(hsv.z + valueOffset);

    return hsv2rgb_smooth(hsv);
}

#pragma include "include/footer.glsl"