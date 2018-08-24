#pragma include "../include/header.glsl"

uniform sampler2DRect mask;
uniform sampler2DRect secondTex;
uniform float threshold = 0.5;

vec4 mainFrag()
{
    float amount = texture(mask, textureCoordinate).r;
    if (amount > threshold) {
        return texture(secondTex, textureCoordinate);
    } else {
        return texture(tex0, textureCoordinate);
    }
}

#pragma include "../include/footer.glsl"
