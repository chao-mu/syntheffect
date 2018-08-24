#pragma include "../include/header.glsl"

uniform float zoom = 1.;
uniform float moveX = 0.;
uniform float moveY = 0.;

vec4 mainFrag() {
    vec2 uv = normalize_1to1(textureCoordinate, resolution);
    
    uv /= zoom;
    
    uv.x += moveX;
    uv.y += moveY;

    uv = denormalize_1to1(uv, resolution);

    return texture(tex0, uv);
}

#pragma include "../include/footer.glsl"
