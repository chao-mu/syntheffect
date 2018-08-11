#pragma include "../include/header.glsl"

uniform sampler2DRect mask;
uniform sampler2DRect secondTex;
uniform float threshold = 0.5;

vec3 mainFrag()
{
    float amount = luminance(texture(mask, textureCoordinate).rgb);
    if (amount > threshold) {
        return texture(secondTex, textureCoordinate).rgb;
    } else {
        return texture(tex0, textureCoordinate).rgb;
    }
}

#pragma include "../include/footer.glsl"
