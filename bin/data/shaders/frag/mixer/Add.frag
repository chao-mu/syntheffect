#pragma include "../include/header.glsl"

uniform sampler2DRect secondTex;

uniform bool addAlpha = false;
uniform bool addRGB = true;

vec4 mainFrag()
{
    vec4 left = texture(tex0, textureCoordinate);
    vec4 right = texture(secondTex, textureCoordinate);
    
    if (addAlpha) {
        left.a += right.a
    }
    
    if (addRGB) {
        left.rgb += right.rgb;
    }

    return left;
}

#pragma include "../include/footer.glsl"
