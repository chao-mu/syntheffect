#pragma include  "../include/header.glsl"

uniform float similarity = 0.001;
uniform sampler2DRect matte;

uniform bool sampleRegion = true;

#define REGION_SIZE 9

DEFINE_GET_3X3(getMatte3x3, matte)

float calcAlpha(float current, vec4[REGION_SIZE] m, vec4[REGION_SIZE] tex) {
    float diff = 0.;
    for (int i = 0; i < REGION_SIZE; i++) {
        diff += abs(m[i].r - tex[i].r);
        diff += abs(m[i].g - tex[i].g);
        diff += abs(m[i].b - tex[i].b);
    }
    
    diff /= REGION_SIZE;

    return diff < similarity ? 0. : current;
}

float calcAlpha(float current, vec4 m, vec4 tex) {
    float diff = 0.;
    diff += abs(m.r - tex.r);
    diff += abs(m.g - tex.g);
    diff += abs(m.b - tex.b);

    return diff < similarity ? 0. : current;
}

void main() {
    vec4 color = texture(tex0, textureCoordinate);
    if (sampleRegion) {
        color.a = calcAlpha(color.a, get3x3(), getMatte3x3());
    } else {
        color.a = calcAlpha(color.a, color, texture(matte, textureCoordinate));
    }
    
    outputColor = color;
}
