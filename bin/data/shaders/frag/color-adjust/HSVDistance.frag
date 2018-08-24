#pragma include "../include/header.glsl"

uniform int channel = 2;
uniform float target = 0.;

vec4 mainFrag()
{
    vec4 color = texture(tex0, textureCoordinate);
    return vec4(abs(target - rgb2hsv(color.rgb)[channel]), color.a);
}

#pragma include "../include/footer.glsl"
