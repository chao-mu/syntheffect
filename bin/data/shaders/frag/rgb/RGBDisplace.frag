#pragma include "../include/header.glsl"

uniform float redOffsetY = 0.;
uniform float redOffsetX = 0.;
uniform float redScale = 1.;

uniform float greenOffsetX = 0.;
uniform float greenOffsetY = 0.;
uniform float greenScale = 1.;

uniform float blueOffsetX = 0.;
uniform float blueOffsetY = 0.;
uniform float blueScale = 1.;

float getOffset(vec3 base, vec2 uv, float scale, float distanceX, float distanceY, int channel) {
    uv *= scale;
    uv.x += (scale * distanceX); 
    uv.y += (scale * distanceY);
    
    if (uv.x > 1. || uv.x < -1. || uv.y > 1. || uv.y < -1.) {
        return 0.;
    }
    
    return texture(tex0, denormalize_1to1(uv, resolution))[channel];
}

vec4 mainFrag() {
    vec2 uv = normalize_1to1(textureCoordinate, resolution);

    vec4 color = texture(tex0, textureCoordinate);

    color.r = getOffset(color, uv, redScale, redOffsetX, redOffsetY, 0);
    color.g = getOffset(color, uv, greenScale, greenOffsetX, greenOffsetY, 1);
    color.b = getOffset(color, uv, blueScale, blueOffsetX, blueOffsetY, 2);

    return color;
}

#pragma include "../include/footer.glsl"
