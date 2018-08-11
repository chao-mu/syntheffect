#pragma include "../include/header.glsl"

uniform float xCount = 1.;
uniform float yCount = 1.;

vec3 mainFrag() {
    vec2 texCoord = textureCoordinate;

    float xOffset = abs(xCount);
    if (xOffset > 0.) {
        texCoord.x = floor(texCoord.x / xOffset) * xOffset;
    }
    
    float yOffset = abs(yCount);
    if (yOffset > 0.) {
        texCoord.y = floor(texCoord.y / yOffset) * yOffset;
    }
    
    return texture(tex0, texCoord).rgb;
}

#pragma include "../include/footer.glsl"
