#pragma include "../include/header.glsl"

uniform float value;
uniform int channel;

void mainFrag(inout vec4 col) {
    col[channel] = value;
}

#pragma include "../include/footer.glsl"
