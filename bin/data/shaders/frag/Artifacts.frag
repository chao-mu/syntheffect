#pragma include "include/module.glsl"

#pragma include "../3rdparty/ColorSpaces.inc.glsl"

DEFINE_INPUT(red, 0., DESC("red component"))
DEFINE_INPUT(green, 0., DESC("green component"))
DEFINE_INPUT(blue, 0., DESC("blue component"))
DEFINE_INPUT(blood, 1., DESC("how much bleed yo want"))

DEFINE_OUTPUT_1(red, DESC("red component"))
DEFINE_OUTPUT_2(green, DESC("green component"))
DEFINE_OUTPUT_3(blue, DESC("blue component"))

// Inspired by https://bitjammer.github.io/glsl/ntsc1.html
void main() {
    vec3 yuv = rgb_to_ycbcr(vec3(input_red(), input_green(), input_blue()));

    vec2 uv = textureCoordinate - vec2(0, 1);
    vec3 yuv2 = rgb_to_ycbcr(vec3(input_red(uv), input_green(uv), input_blue(uv)));

    float blood_factor = input_blood();
    float luma_blood  = (yuv.x - yuv2.x) * blood_factor;
    float chroma_blood = length(yuv.yz - yuv.yz) * blood_factor;

    yuv.yz += vec2(luma_blood, luma_blood);
    yuv.x += chroma_blood;

    vec3 rgb = ycbcr_to_rgb(yuv);
    output_red(rgb.r);
    output_green(rgb.g);
    output_blue(rgb.b);
}
