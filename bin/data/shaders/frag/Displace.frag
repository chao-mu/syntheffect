#pragma include "include/module.glsl"

DEFINE_INPUT(in1, 0., DESC("input channel 1"))
DEFINE_INPUT(in2, 0., DESC("input channel 2"))
DEFINE_INPUT(in3, 0., DESC("input channel 3"))

DEFINE_INPUT(displaceH, 0., DESC("displacement signal for horizontal displacement, value is 0.5 - X "))
DEFINE_INPUT(displaceV, 0., DESC("displacement signal for vertical displacement, value is 0.5 - X "))
DEFINE_INPUT(displaceGain, 1., DESC("multiplies displaceH and displaceV"))

DEFINE_OUTPUT_1(out1, DESC("output channel for in1"))
DEFINE_OUTPUT_2(out2, DESC("output channel for in2"))
DEFINE_OUTPUT_3(out3, DESC("output channel for in3"))

void main() {
    vec2 uv = get_uv_1to1();

    uv += (vec2(0.5) - vec2(input_displaceH(), input_displaceV())) * input_displaceGain();
    vec2 coords = from_uv_1to1(uv);

    output_out1(input_in1(coords));
    output_out2(input_in2(coords));
    output_out3(input_in3(coords));
}
