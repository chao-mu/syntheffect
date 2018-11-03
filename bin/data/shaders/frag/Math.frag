#pragma include "include/module.glsl"

DEFINE_INPUT(in1, 0, DESC("first component of vector"))
DEFINE_INPUT(in2, 0, DESC("second component of vector"))
DEFINE_INPUT(in3, 0, DESC("third component of vector"))

DEFINE_OUTPUT_1(value, DESC("Resulting magnitude"))

void main() {
    output_value(length(vec3(input_in1(), input_in2(), input_in3())));
}
