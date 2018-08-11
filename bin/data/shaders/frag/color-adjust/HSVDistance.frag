#pragma include "../include/header.glsl"

uniform int channel = 2;
uniform float target = 0.;

vec3 mainFrag()
{
    return vec3(abs(target - rgb2hsv(texture(tex0, textureCoordinate).rgb)[channel]));
}

#pragma include "../include/footer.glsl"
