#pragma include "include/module.glsl"
#pragma include "include/kernels.glsl"
#pragma include "../3rdparty/ColorSpaces.inc.glsl"

DEFINE_INPUT(red, 0., DESC("red component"))
DEFINE_INPUT(green, 0., DESC("green component"))
DEFINE_INPUT(blue, 0., DESC("blue component"))
DEFINE_INPUT(negate, 0, DESC("negate the result"))

DEFINE_INPUT_GROUP(rgb, red, green, blue)

DEFINE_OUTPUT_1(red, DESC("red component"))
DEFINE_OUTPUT_2(green, DESC("green component"))
DEFINE_OUTPUT_3(blue, DESC("blue component"))

DEFINE_OUTPUT_GROUP(rgb, red, green, blue)

void main() {
    vec3 n[9] = input_3x3_rgb();
    vec3 a = applyKernel(KERNEL_SCHARR_X, n);
    vec3 b = applyKernel(KERNEL_SCHARR_Y, n);
	vec3 edgeRGB = sqrt((a * a) + (b * b));

    if (is_true(input_negate())) {
        edgeRGB = vec3(1) - edgeRGB;
    }

    output_rgb(edgeRGB);
}
