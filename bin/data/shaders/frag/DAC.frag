#pragma include "include/module.glsl"

DEFINE_INPUT(in1, 0, DESC("input channel for bit 1"))
DEFINE_INPUT(in2, 0, DESC("input channel for bit 2"))
DEFINE_INPUT(in3, 0, DESC("input channel for bit 3"))

DEFINE_OUTPUT_1(value, DESC("Resulting composed value"))

void main() {
    float v = 0;

    v += input_in1() *  0.125;
    v += input_in2() *  0.375;
    v += input_in3() *  0.5;

    output_value(v);
}
