#pragma include "include/module.glsl"

DEFINE_INPUT(red, 0., DESC("red component"))
DEFINE_INPUT(green, 0., DESC("green component"))
DEFINE_INPUT(blue, 0., DESC("blue component"))

DEFINE_INPUT(key, 0., DESC("whether left or right texture is used"))
DEFINE_INPUT(keyThreshold, 0.5, DESC("the number dictating what 'key' value keys left/right"))
DEFINE_INPUT(keyNegate, 0., DESC("If above 0.5, negates the keying"))
DEFINE_INPUT(keyMixMode, 0., DESC("If above 0.5, mixes instead of keys"))

DEFINE_INPUT(red2, 0., DESC("red component"))
DEFINE_INPUT(green2, 0., DESC("green component"))
DEFINE_INPUT(blue2, 0., DESC("blue component"))

DEFINE_OUTPUT_1(red, DESC("red component"))
DEFINE_OUTPUT_2(green, DESC("green component"))
DEFINE_OUTPUT_3(blue, DESC("blue component"))

void main() {
    vec3 rgb_left = vec3(input_red(), input_green(), input_blue());
    vec3 rgb_right = vec3(input_red2(), input_green2(), input_blue2());

    float key = input_key();
    if (input_keyNegate() > 0.5) {
        key = 1.0 - key;
    }

    float key_thresh = input_keyThreshold();
    if (key > key_thresh) {
        if (input_keyMixMode() > 0.5) {
            rgb_left = mix(rgb_left, rgb_right, key - key_thresh);
        } else {
            rgb_left = rgb_right;
        }
    }

    output_red(rgb_left.r);
    output_green(rgb_left.g);
    output_blue(rgb_left.b);
}
