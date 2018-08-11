#pragma include "../include/header.glsl"

uniform float redOffsetY = 0.;
uniform float redOffsetX = 0.;

uniform float greenOffsetX = 0.;
uniform float greenOffsetY = 0.;

uniform float blueOffsetX = 0.;
uniform float blueOffsetY = 0.;

vec4 getNormalized(vec2 uv) {
    return texture(tex0, denormalize_1to1(uv, resolution));
}

float getOffset(vec3 base, vec2 uv, float distanceX, float distanceY, int channel) {
    uv.x += distanceX; 
    uv.y += distanceY;
    
    return mix(base[channel], getNormalized(uv)[channel], 0.5);
}

vec3 mainFrag() {
    vec2 uv = normalize_1to1(textureCoordinate, resolution);

    vec3 color = texture(tex0, textureCoordinate).rgb;

    color.r = getOffset(color, uv, redOffsetX, redOffsetY, 0);
    color.g = getOffset(color, uv, greenOffsetX, greenOffsetY, 1);
    color.b = getOffset(color, uv, blueOffsetX, blueOffsetY, 2);

    return color;
}

#pragma include "../include/footer.glsl"
