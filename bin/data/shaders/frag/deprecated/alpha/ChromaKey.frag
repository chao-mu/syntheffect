#pragma include  "../include/header.glsl"

uniform float red = 0.;
uniform float green = 0.;
uniform float blue = 1.;

uniform float similarity = 0.01;
uniform float blend = 0.;

#define REGION_SIZE 9

// frag and target are in the uv of yuv (chroma).
float calcAlpha(float current, vec4[REGION_SIZE] region, vec3 target) {
    float diff = 0.;
    for (int i = 0; i < REGION_SIZE; i++) {
        vec3 yuv = yuv_to_01(srgb_to_yuv(region[i]));
        float du = yuv.y - target.y;
        float dv = yuv.z - target.z;
        
        diff += sqrt((du * du + dv * dv) / 2.);
    }
    
    diff /= REGION_SIZE;

    return diff > similarity ? current : 0.;
}

void main() {
    vec4 color = texture(tex0, textureCoordinate);
    vec3 target = srgb_to_yuv(vec3(red, green, blue));
    color.a = calcAlpha(color.a, get3x3(), target);
    
    outputColor = color;
}
