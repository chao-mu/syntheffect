#pragma include "include/module.glsl"

DEFINE_INPUT(freq, 10, DESC("frequency of wave"))
DEFINE_INPUT(freqMod, 0, DESC("frequency modulation of wave"))
DEFINE_INPUT(amplitude, 1, DESC("ampply resulting wave"))
DEFINE_INPUT(shift, 0, DESC("offset of wave"))
DEFINE_INPUT(vertical, 0, DESC("vertical or horizontal"))
DEFINE_INPUT(phase, NO_DEFAULT, DESC("phase of wave"))

DEFINE_OUTPUT_1(sine, DESC("sine wave"))
DEFINE_OUTPUT_2(sawtooth, DESC("sawtooth wave"))
DEFINE_OUTPUT_3(arcsine, DESC("anti sine wave"))

void main() {
    float freq = input_freq();
    float amp = input_amplitude();
    float shift = input_shift();

    float phase;
    if (passed_phase()) {
        phase = input_phase();
    } else if (input_vertical() > 0.5) {
        phase = (textureCoordinate.y + input_nosync_syncV(textureCoordinate)) / resolution.y;
    } else {
        phase = (textureCoordinate.x + input_nosync_syncH(textureCoordinate)) / resolution.x;
    }

    float x = freq * phase + shift + input_freqMod();

    output_sine(amp * (0.5 * sin(x) + 0.5));
    output_sawtooth(amp * mod(x, 1.0));
    output_arcsine(amp * map(abs(asin(2 * mod(x, 1.) - 1 )), 0, HALF_PI, 0, 1));
}
