#pragma include "include/module.glsl"

DEFINE_INPUT(in1, 0., DESC("input channel 1"))
DEFINE_INPUT(in2, 0., DESC("input channel 2"))
DEFINE_INPUT(in3, 0., DESC("input channel 3"))

DEFINE_INPUT(amount, 0.8, DESC("amount the previous frame will be mixed in with the current"))

DEFINE_OUTPUT_1(out1, DESC("output channel for in1"))
DEFINE_OUTPUT_2(out2, DESC("output channel for in2"))
DEFINE_OUTPUT_3(out3, DESC("output channel for in3"))

void main() {
    vec3 last = input_accumulator();
    vec3 current = vec3(input_in1(), input_in2(), input_in3());
    float amount = firstPass ? 0 : input_amount();
    vec3 next = mix(current, last, amount);

    output_accumulator(next);

    output_out1(next.x);
    output_out2(next.y);
    output_out3(next.z);
}
