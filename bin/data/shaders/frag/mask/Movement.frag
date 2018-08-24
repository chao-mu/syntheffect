#pragma include "../include/header.glsl"

uniform sampler2DRect last;

vec4 mainFrag() {
    vec4 currentCol = texture(tex0, textureCoordinate);
    vec4 lastCol = texture(last, textureCoordinate);

    return vec4(abs(currentCol.rgb - lastCol.rgb), currentCol.a);
}

#pragma include "../include/footer.glsl"
