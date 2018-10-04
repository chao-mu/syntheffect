#pragma include "../include/header.glsl"

uniform float low = 0;
uniform float high = 1;

void mainFrag(inout vec4 col) {
    col.rgb = smoothstep(low, high, col.rgb);
}

#pragma include "../include/footer.glsl"
