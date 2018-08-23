#pragma include "../include/header.glsl"

uniform sampler2DRect secondTex;

vec3 mainFrag() {
    vec4 under = texture(tex0, textureCoordinate);
    vec4 over = texture(secondTex, textureCoordinate);

    return mix(under.rgb, over.rgb, over.a);
}

#pragma include "../include/footer.glsl"
