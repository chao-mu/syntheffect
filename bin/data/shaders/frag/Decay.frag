#pragma include "include/module.glsl"

DEFINE_INPUT(in1, 0., DESC("input channel 1"))
DEFINE_INPUT(in2, 0., DESC("input channel 2"))
DEFINE_INPUT(in3, 0., DESC("input channel 3"))

DEFINE_INPUT_GROUP(ins, in1, in2, in3)

DEFINE_INPUT(amount, 0.8, DESC("amount the previous frame will be mixed in with the current"))

DEFINE_OUTPUT_1(out1, DESC("output channel for in1"))
DEFINE_OUTPUT_2(out2, DESC("output channel for in2"))
DEFINE_OUTPUT_3(out3, DESC("output channel for in3"))

DEFINE_OUTPUT_GROUP(outs, out1, out2, out3)

void main() {
    vec3 last = last_output_outs();
    vec3 ins = input_ins();
    float amount = input_amount();

    vec3 outs;
    outs.x = ins.x > last.x ? ins.x : mix(ins.x, last.x, amount);
    outs.y = ins.y > last.y ? ins.y : mix(ins.y, last.y, amount);
    outs.z = ins.z > last.z ? ins.z : mix(ins.z, last.z, amount);

    output_outs(outs);
}
