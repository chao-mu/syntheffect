#pragma include  "../include/header.glsl"

uniform float red = 0.;
uniform float green = 0.;
uniform float blue = 1.;

uniform float similarity = 0.01;
uniform float blend = 0.;

#define REGION_SIZE 9

// frag and target are in the uv of yuv (chroma).
float calcAlpha(float current, vec3[REGION_SIZE] regionRGB, vec2 targetUV) {
    float diff = 0.;
    for (int i = 0; i < REGION_SIZE; i++) {
        float du = RGB_TO_U(regionRGB[i]) - targetUV[0];
        float dv = RGB_TO_V(regionRGB[i]) - targetUV[1];
        
        diff += sqrt((du * du + dv * dv) / 2.);
    }
    
    diff /= REGION_SIZE;

    return diff > similarity ? current : 0.;
}

void main() {
    vec4 color = texture(tex0, textureCoordinate);
    vec2 target = rgb2chroma(vec3(red, green, blue));
    color.a = calcAlpha(color.a, get3x3(), target);
    
    outputColor = color;
}
