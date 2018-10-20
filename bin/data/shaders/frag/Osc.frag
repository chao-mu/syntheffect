#pragma include "include/module.glsl"

DEFINE_INPUT(freq, 1, DESC("frequency of wave"))
DEFINE_INPUT(multiplier, 1, DESC("multiply resulting wave"))
DEFINE_INPUT(shift, 0, DESC("offset of wave"))
DEFINE_INPUT(vertical, 0, DESC("vertical or horizontal"))

DEFINE_OUTPUT_1(sine, DESC("sine wave"))
DEFINE_OUTPUT_2(triangle, DESC("triangle wave"))
DEFINE_OUTPUT_3(square, DESC("square wave"))
DEFINE_OUTPUT_4(sawtooth, DESC("sawtooth wave"))

void main() {
    float freq = input_freq();
    float multi = input_multiplier();
    float shift = input_shift();
    float phase = (input_vertical() > 0.5 ? textureCoordinate.y : textureCoordinate.x) / 10.;

    float x = freq * phase + shift;

    output_sine(multi * (0.5 * sin(x) + 0.5));
    output_sawtooth(multi * mod(x, 1.0));
}
