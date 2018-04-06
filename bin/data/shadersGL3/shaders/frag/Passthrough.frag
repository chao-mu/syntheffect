#pragma include "include/header.glsl"

vec3 mainFrag() {
    return texture(tex0, textureCoordinate).rgb;
}

#pragma include "include/footer.glsl"