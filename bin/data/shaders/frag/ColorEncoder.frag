#pragma include "include/module.glsl"

DEFINE_INPUT(red, 0., DESC("red component"))
DEFINE_INPUT(redInvert, 0., DESC("red component, at 0 non-inverted, at 1 inverted"))
DEFINE_INPUT(redBias, 0., DESC("red component bias"))

DEFINE_INPUT(green, 0., DESC("green component"))
DEFINE_INPUT(greenInvert, 0., DESC("green component, at 0 non-inverted, at 1 inverted"))
DEFINE_INPUT(greenBias, 0., DESC("green component bias"))

DEFINE_INPUT(blue, 0., DESC("blue component"))
DEFINE_INPUT(blueInvert, 0., DESC("blue component, at 0 non-inverted, at 1 inverted"))
DEFINE_INPUT(blueBias, 0., DESC("blue component bias"))

DEFINE_OUTPUT_1(red, DESC("red component"))
DEFINE_OUTPUT_2(green, DESC("green component"))
DEFINE_OUTPUT_3(blue, DESC("blue component"))

float encode(float col, float invert, float bias) {
    return mix(col, 1. - col, invert) + bias;
}

void main() {
    output_red(encode(input_red(), input_redInvert(), input_redBias()));
    output_green(encode(input_green(), input_greenInvert(), input_greenBias()));
    output_blue(encode(input_blue(), input_blueInvert(), input_blueBias()));
}
