#pragma include "../include/header.glsl"

uniform sampler2DRect last;

vec3 mainFrag() {
    vec3 currentCol = texture(tex0, textureCoordinate).rgb;
    vec3 lastCol = texture(last, textureCoordinate).rgb;

    return abs(currentCol - lastCol);
}

#pragma include "../include/footer.glsl"
