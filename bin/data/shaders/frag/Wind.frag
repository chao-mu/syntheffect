#pragma include "include/module.glsl"

DEFINE_INPUT(in1, 0., DESC("input channel 1"))
DEFINE_INPUT(in2, 0., DESC("input channel 2"))
DEFINE_INPUT(in3, 0., DESC("input channel 3"))
DEFINE_INPUT(amount, 1, DESC("amount of winding"))

DEFINE_INPUT_GROUP(ins, in1, in2, in3)

DEFINE_OUTPUT_1(out1, DESC("output channel for in1"))
DEFINE_OUTPUT_2(out2, DESC("output channel for in2"))
DEFINE_OUTPUT_3(out3, DESC("output channel for in3"))

DEFINE_OUTPUT_GROUP(outs, out1, out2, out3)

void main() {
    vec2 uv = get_uv_1to1();

    // [0, 1]
    float radius = map(length(uv), 0, SQRT_2, 0, 1);
    // [0, 1]
    float angle = map(atan(uv.y, uv.x), -PI, PI, 0, 1);

    vec2 polar = vec2(radius, angle);

    vec2 coords = from_uv_0to1(mix(get_uv_0to1(), polar, input_amount()));

    output_outs(input_ins(coords));
}
