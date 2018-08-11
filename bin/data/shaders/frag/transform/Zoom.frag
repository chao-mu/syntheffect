#pragma include "../include/header.glsl"

uniform float factor = 1.;
uniform float speed = 1.;

vec3 mainFrag() {
    vec2 uv = normalize_1to1(textureCoordinate, resolution);

    uv /= factor;

    uv = denormalize_1to1(uv, resolution);

    return texture(tex0, uv).rgb;
}

#pragma include "../include/footer.glsl"
