#pragma include  "../include/header.glsl"

uniform float luma;

uniform float similarity = 0.01;
uniform bool measureSpill = false;

#define REGION_SIZE 9

float diffLuma(vec4[REGION_SIZE] regionRGB, float targetLuma) {
    float diff = 0.;
    for (int i = 0; i < REGION_SIZE; i++) {
        diff += abs(RGB_TO_Y(regionRGB[i].rgb) - targetLuma);
    }
    
    return diff / REGION_SIZE;
}

void mainFrag(inout vec4 col) {
    float diff = diffLuma(get3x3(), luma);
    
    if (measureSpill) {
        col.a = diff - similarity <  0 ? 0 : diff;
    } else if (diff < similarity) {
        col.a = 0.;
    }
}

#pragma include  "../include/footer.glsl"
