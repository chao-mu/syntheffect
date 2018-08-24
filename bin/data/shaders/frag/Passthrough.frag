#pragma include "include/header.glsl"

vec4 mainFrag() {
    return texture(tex0, textureCoordinate);
}

#pragma include "include/footer.glsl"
