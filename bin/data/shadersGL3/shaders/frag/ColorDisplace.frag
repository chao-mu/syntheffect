#pragma include "include/header.glsl"

float distance = 0.05;

vec4 getNormalized(vec2 uv) {
    return texture(tex0, uv * resolution);
}

float noiseyChannel(vec3 base, vec2 uv, int channel) {
    uv.x += mix(-distance, distance, noise(time + ((channel + 1) * 5)));
    uv.y += mix(-distance, distance, noise(time + ((channel + 1) * 7)));
    return mix(base[channel], getNormalized(uv)[channel], 0.5);
}

vec3 mainFrag() {
    vec2 uv = textureCoordinate / resolution;

    vec3 color = texture(tex0, textureCoordinate).rgb;

    color.r = noiseyChannel(color, uv, 0);
    color.g = noiseyChannel(color, uv, 1);
    color.b = noiseyChannel(color, uv, 2);

    return color;
}

#pragma include "include/footer.glsl"
