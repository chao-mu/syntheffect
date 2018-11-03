#pragma include "include/module.glsl"

DEFINE_INPUT(value, 0., DESC("input signal to be classified"))
DEFINE_INPUT(invert, 0., DESC("0 is non inverted, 1 is inverted"))
DEFINE_INPUT(bias, 0, DESC("Adjustment to input signal"))

DEFINE_OUTPUT_1(out1, DESC("output channel for bit 1"))
DEFINE_OUTPUT_2(out2, DESC("output channel for bit 2"))
DEFINE_OUTPUT_3(out3, DESC("output channel for bit 3"))

void main() {
    float v = input_value();

    v = mix(v, 1.0 - v, input_invert());
    v += input_bias();

    if (v > 0.875) {
        output_out1(1.0);
        output_out2(1.0);
        output_out3(1.0);
    } else if (v > 0.750) {
        output_out1(0.0);
        output_out2(1.0);
        output_out3(1.0);
    } else if (v > 0.625) {
        output_out1(1.0);
        output_out2(0.0);
        output_out3(1.0);
    } else if (v > 0.5) {
        output_out1(0.0);
        output_out2(0.0);
        output_out3(1.0);
    } else if (v > 0.375) {
        output_out1(1.0);
        output_out2(1.0);
        output_out3(0.0);
    } else if (v > 0.25) {
        output_out1(0.0);
        output_out2(1.0);
        output_out3(0.0);
    } else if (v > 0.125) {
        output_out1(1.0);
        output_out2(0.0);
        output_out3(0.0);
    } else {
        output_out1(0.0);
        output_out2(0.0);
        output_out3(0.0);
    }
}
