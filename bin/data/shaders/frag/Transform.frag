#pragma include "include/module.glsl"

DEFINE_INPUT(in1, 0., DESC("input channel 1"))
DEFINE_INPUT(in2, 0., DESC("input channel 2"))
DEFINE_INPUT(in3, 0., DESC("input channel 3"))

DEFINE_INPUT(mirrorX, 0, DESC("Mirror x axis"))
DEFINE_INPUT(mirrorY, 0, DESC("Mirror x axis"))

DEFINE_INPUT_GROUP(ins, in1, in2, in3)

DEFINE_OUTPUT_1(out1, DESC("output channel for in1"))
DEFINE_OUTPUT_2(out2, DESC("output channel for in2"))
DEFINE_OUTPUT_3(out3, DESC("output channel for in3"))

DEFINE_OUTPUT_GROUP(outs, out1, out2, out3)

void main() {
    vec2 uv = get_uv_1to1();

    if (is_true(input_mirrorX())) {
        uv.x = abs(uv.x);
    }

    if (is_true(input_mirrorY())) {
        uv.y = abs(uv.y);
    }

    output_outs(input_ins(from_uv_1to1(uv)));
}
