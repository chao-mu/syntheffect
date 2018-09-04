#pragma include  "../include/header.glsl"

uniform float luma;

uniform float similarity = 0.01;

#define REGION_SIZE 9

float calcAlpha(float current, vec3[REGION_SIZE] regionRGB, float targetLuma) {
    float diff = 0.;
    for (int i = 0; i < REGION_SIZE; i++) {
        diff += abs(RGB_TO_Y(regionRGB[i]) - targetLuma);
    }
    
    diff /= REGION_SIZE;

    return diff > similarity ? current : 0.;
}

void main() {
    vec4 color = texture(tex0, textureCoordinate);
    color.a = calcAlpha(color.a, get3x3(), luma);
    
    outputColor = color;
}
