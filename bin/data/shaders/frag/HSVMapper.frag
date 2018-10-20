#pragma include "include/module.glsl"

#pragma include "../3rdparty/ColorSpaces.inc.glsl"

DEFINE_INPUT(hue, 0., DESC("hue component"))
DEFINE_INPUT(saturation, 0., DESC("saturation component"))
DEFINE_INPUT(value, 0, DESC("value component"))

DEFINE_OUTPUT_1(red, DESC("red component"))
DEFINE_OUTPUT_2(green, DESC("green component"))
DEFINE_OUTPUT_3(blue, DESC("blue component"))

DEFINE_OUTPUT_4(luma, DESC("luma component"))
DEFINE_OUTPUT_5(cb, DESC("blue contribution to luma"))
DEFINE_OUTPUT_6(cr, DESC("red contribution to luma"))

void main() {
    vec3 hsv = vec3(input_hue(), input_saturation(), input_value());

    vec3 rgb = hsv_to_rgb(hsv);
    output_red(rgb.r);
    output_green(rgb.g);
    output_blue(rgb.b);

    vec3 yuv = hsv_to_ycbcr(hsv);
    output_luma(yuv.x);
    output_cb(yuv.y);
    output_cr(yuv.z);
}

