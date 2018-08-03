#pragma include "include/header.glsl"

uniform sampler2DRect secondTex;
uniform float divider = 0.5;

uniform bool horizontal = false;

vec3 mainFrag()
{
    vec2 uv = normalize_0to1(textureCoordinate, resolution);
    float x = horizontal ? uv.y : uv.x;
    
    if (x < divider) {
        return texture(tex0, textureCoordinate).rgb;
    } else {
        return texture(secondTex, textureCoordinate).rgb;
    }
}

#pragma include "include/footer.glsl"
