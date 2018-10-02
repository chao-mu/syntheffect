#pragma include "../include/header.glsl"

uniform float rotation = 0;

void mainFrag(inout vec4 col) {
    vec2 uv = normalize_1to1(textureCoordinate, resolution);

    uv *= rotate(rotation);

    uv = denormalize_1to1(uv, resolution);

    col = texture(tex0, uv);
}

#pragma include "../include/footer.glsl"
