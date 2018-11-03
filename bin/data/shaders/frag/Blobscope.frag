#pragma include "include/module.glsl"

// Multiple the result of this function call to rotate the coordinates by the given angle.
#define rotate(angle) mat2(cos(angle),-sin(angle), sin(angle),cos(angle));

DEFINE_INPUT(speed, 0.5, DESC("speed"))
DEFINE_INPUT(shapes, 20., DESC("corelates to number of shapes"))
DEFINE_INPUT(sharpness, 2.5, DESC("Sharpness of image"))

DEFINE_OUTPUT_1(value, DESC("the image"))

void main() {
    // -1 to 1
    vec2 uv = get_uv_1to1();

    float speed = input_speed();
    float shapes = input_shapes();
    float sharpness = input_sharpness();

    float value = 0.;
    for (float i=0.; i < 6.; i += 1.) {
    	uv *= rotate(time / 10.)

        float dx = length(uv.x);
        float dy = length(uv.y);

        value += sin((dx - (time * float(speed))) * shapes) * sharpness;
        value += sin((dy - (time * float(speed))) * shapes / 2.) * sharpness;
    }

    output_value(value);
}
