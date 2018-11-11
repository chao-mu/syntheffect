#pragma include "include/module.glsl"

DEFINE_INPUT(in1, 0., DESC("input channel 1"))
DEFINE_INPUT(in2, 0., DESC("input channel 2"))
DEFINE_INPUT(in3, 0., DESC("input channel 3"))

DEFINE_INPUT(amountX, 0.3, DESC("Amount chunked along X"))
DEFINE_INPUT(amountY, 0.3, DESC("Amount chunked along Y"))

DEFINE_INPUT_GROUP(ins, in1, in2, in3)

DEFINE_OUTPUT_1(out1, DESC("output channel for in1"))
DEFINE_OUTPUT_2(out2, DESC("output channel for in2"))
DEFINE_OUTPUT_3(out3, DESC("output channel for in3"))

DEFINE_OUTPUT_GROUP(outs, out1, out2, out3)

void main() {
    vec2 coord = textureCoordinate;

    float xOffset = resolution.x * input_amountX() / 18.;
    if (xOffset > 0.00001) {
        coord.x = floor(coord.x / xOffset) * xOffset;
    }

    float yOffset = resolution.x * input_amountY() / 18.;
    if (yOffset > 0.00001) {
        coord.y = floor(coord.y / yOffset) * yOffset;
    }

    output_outs(input_ins(coord));
}
