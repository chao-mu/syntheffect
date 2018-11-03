#pragma include "include/module.glsl"

DEFINE_INPUT(in1, 0, DESC("first component of vector"))
DEFINE_INPUT(in2, 0, DESC("second component of vector"))
DEFINE_INPUT(in3, 0, DESC("third component of vector"))
DEFINE_INPUT(new, 1, DESC("whether there is new input"))

DEFINE_OUTPUT_1(value, DESC("Resulting magnitude of difference"))
DEFINE_OUTPUT_2(in1_, DESC("first component of vector"))
DEFINE_OUTPUT_3(in2_, DESC("second component of vector"))
DEFINE_OUTPUT_4(in3_, DESC("third component of vector"))

void main() {
    vec3 current = vec3(input_in1(), input_in2(), input_in3());
    vec3 last_ins = vec3(last_output_in1_(), last_output_in2_(), last_output_in3_());

    float value = input_new() > 0.5 ? length(last_ins - current) : last_output_value();

    output_in1_(current.x);
    output_in2_(current.y);
    output_in3_(current.z);
    output_value(value);
}
