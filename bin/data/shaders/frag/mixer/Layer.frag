#pragma include "../include/header.glsl"

uniform sampler2DRect secondTex;

vec4 mainFrag() {
    vec4 under = texture(tex0, textureCoordinate);
    vec4 over = texture(secondTex, textureCoordinate);

    return vec4(mix(under.rgb, over.rgb, over.a), clamp(under.a + over.a, 0, 1));
}

#pragma include "../include/footer.glsl"
