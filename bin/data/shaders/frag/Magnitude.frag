#pragma include "include/module.glsl"

DEFINE_INPUT(x1, 0, DESC("first component of vector"))
DEFINE_INPUT(x2, 0, DESC("second component of vector"))
DEFINE_INPUT(x3, 0, DESC("third component of vector"))

DEFINE_INPUT(y1, 0, DESC("first component of vector"))
DEFINE_INPUT(y2, 0, DESC("second component of vector"))
DEFINE_INPUT(y3, 0, DESC("third component of vector"))

DEFINE_OUTPUT_1(value, DESC("Resulting magnitude of x"))
DEFINE_OUTPUT_2(value_diff, DESC("Resulting magnitude of x - y"))

void main() {
    vec3 x = vec3(input_x1(), input_x2(), input_x3());
    vec3 y = vec3(input_y1(), input_y2(), input_y3());

    output_value_diff(length(x - y));
    output_value(length(x));
}
