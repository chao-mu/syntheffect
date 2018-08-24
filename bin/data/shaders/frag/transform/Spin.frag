#pragma include "../include/header.glsl"

uniform float speed = 1.;

vec4 mainFrag() {
    vec2 uv = normalize_1to1(textureCoordinate, resolution);

    uv *= rotate(time * speed);

    uv = denormalize_1to1(uv, resolution);

    return texture(tex0, uv);
}

#pragma include "../include/footer.glsl"
