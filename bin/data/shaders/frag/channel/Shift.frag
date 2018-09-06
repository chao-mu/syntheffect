#pragma include "../include/header.glsl"

uniform float shift = 0.;
uniform int channel;

#define EPSILON 1.0e-10

vec4 mainFrag() {
    vec4 color = texture(tex0, textureCoordinate);

    color[channel] = fract(color[channel] + shift);

    return color;
}

#pragma include "../include/footer.glsl"
