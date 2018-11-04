#pragma include "../include/header.glsl"

uniform float factor = 1.;
uniform float speed = 1.;

uniform float fillChannel1 = 0;
uniform float fillChannel2 = 0;
uniform float fillChannel3 = 0;
uniform float fillChannel4 = 0;

void mainFrag(inout vec4 col) {
    vec2 uv = normalize_1to1(textureCoordinate, resolution);

    uv /= factor;
    
    uv = denormalize_1to1(uv, resolution);

    if (uv.x > resolution.x || uv.x < 0 || uv.y > resolution.y || uv.y < 0) {
        col = vec4(fillChannel1, fillChannel2, fillChannel3, fillChannel4);
    } else {
        col = texture(tex0, uv);
    }
}

#pragma include "../include/footer.glsl"
