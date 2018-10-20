#pragma include "include/module.glsl"

#pragma include "../3rdparty/ColorSpaces.inc.glsl"

DEFINE_INPUT(red, 0., DESC("red component"))
DEFINE_INPUT(green, 0., DESC("green component"))
DEFINE_INPUT(blue, 0., DESC("blue component"))

DEFINE_OUTPUT_1(luma, DESC("luma component"))
DEFINE_OUTPUT_2(cb, DESC("blue contribution to luma"))
DEFINE_OUTPUT_3(cr, DESC("red contribution to luma"))

DEFINE_OUTPUT_4(hue, DESC("hue component"))
DEFINE_OUTPUT_5(saturation, DESC("saturation component"))
DEFINE_OUTPUT_6(value, DESC("value component"))

void main() {
    vec3 rgb = vec3(input_red(), input_green(), input_blue());

    vec3 ycbcr = rgb_to_ycbcr(rgb);
    output_luma(ycbcr.x);
    output_cb(ycbcr.y);
    output_cr(ycbcr.z);

    vec3 hsv = rgb_to_hsv(rgb);
    output_hue(hsv.x);
    output_saturation(hsv.y);
    output_value(hsv.z);
}
