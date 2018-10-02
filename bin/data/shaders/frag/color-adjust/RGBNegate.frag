#pragma include "../include/header.glsl"

// More information on the topic of inversions: https://github.com/vn971/linux-color-inversion
// Code not from above link.

void mainFrag(inout vec4 col) {
    col.rgb = vec3(1) - col.rgb;
}

#pragma include "../include/footer.glsl"
