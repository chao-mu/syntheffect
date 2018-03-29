#pragma include "common/frag.glsl"

float distance = 0.05;

vec4 getNormalized(vec2 uv) {
    return texture(tex0, uv * resolution);
}

float noiseyChannel(vec4 base, vec2 uv, int channel) {
    uv.x += mix(-distance, distance, noise(time + ((channel + 1) * 5)));
    uv.y += mix(-distance, distance, noise(time + ((channel + 1) * 7)));
    return mix(base[channel], getNormalized(uv)[channel], 0.5);
}

void main() {
    vec2 uv = texture_coord / resolution;

    vec4 color = texture(tex0, uv * resolution);

    color.r = noiseyChannel(color, uv, 0);
    color.g = noiseyChannel(color, uv, 1);
    color.b = noiseyChannel(color, uv, 2);

    outputColor = color;
}