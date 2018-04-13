#pragma include "include/header.glsl"

uniform float speed = 1.;
uniform float range = 2.;
uniform float zoomAdjustment = 0.1;

vec3 mainFrag() {
    vec2 uv = normalize_1to1(textureCoordinate, resolution);

    uv /= abs(sin(time * speed) * range) + zoomAdjustment;

    uv = denormalize_1to1(uv, resolution);

    return texture(tex0, uv).rgb;
}

#pragma include "include/footer.glsl"