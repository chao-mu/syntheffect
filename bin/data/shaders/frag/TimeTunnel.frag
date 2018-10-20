#pragma include "include/module.glsl"

#pragma include "include/noise.glsl"
#pragma include "include/transform.glsl"

DEFINE_INPUT(speed, 2, DESC("the speed of travel through the tunnel"))
DEFINE_INPUT(travelOffset, 0, DESC("offset of travel time"))
DEFINE_INPUT(seed, 0, DESC("offset of travel time"))

DEFINE_OUTPUT_1(value, DESC("value of the tunnel at a location"))

// http://roy.red/slitscan-.html
void main() {
    // [-1, 1]
    vec2 pt = (2. * textureCoordinate - resolution.xy) / resolution.y;

    float t = (time * input_speed()) + (input_travelOffset() * 2);
    float rInv = 1./length(pt);
    pt = (rotate2d(time) * pt) * rInv - vec2(rInv + t, 0.0);

    float v = iqnoise(5.*pt + input_seed() * 100.0,0.,1.);

    output_value(v);
}
