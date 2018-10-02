#pragma include "../include/header.glsl"

uniform sampler2DRect secondTex;
uniform float divider = 0.5;

uniform bool horizontal = false;

void mainFrag(inout vec4 col)
{
    vec2 uv = normalize_0to1(textureCoordinate, resolution);
    float x = horizontal ? uv.y : uv.x;
    
    if (x > divider) {
        col = texture(secondTex, textureCoordinate);
    }
}

#pragma include "../include/footer.glsl"
