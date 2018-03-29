#pragma include "common/frag.glsl"

void main()
{
    vec2 uv = texture_coord / resolution;

    float border = 1.0 - fract(time/10.0);
    if (
        step(border, uv.x) == 0.0 || step(1.0 - border, uv.x) != 0.0 ||
        step(border, uv.y) == 0.0 || step(1.0 - border, uv.y) != 0.0
    ) {
        uv -= vec2(0.5);
        uv *= rotate2d(time);
        uv /= 1.0 + (fract(time/10.0) * 10.0);
        uv += vec2(0.5);
    }

    outputColor = texture(tex0, uv * resolution);
}