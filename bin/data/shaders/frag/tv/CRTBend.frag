#pragma include "../include/header.glsl"

uniform int radius = 9; 

uniform float straightness = 3.2;

vec3 mainFrag() {
    vec2 uv = normalize_1to1(textureCoordinate, resolution);

    uv.x *= 1.0 + pow((abs(uv.y) / straightness), 2.0);
    uv.y *= 1.0 + pow((abs(uv.x) / straightness), 2.0);
    
    return texture(tex0, denormalize_1to1(uv, resolution)).rgb;
}

#pragma include "../include/footer.glsl"

