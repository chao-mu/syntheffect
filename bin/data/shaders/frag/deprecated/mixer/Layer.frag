#pragma include "../include/header.glsl"

uniform sampler2DRect overTex;
uniform vec2 overTex_resolution;

void mainFrag(inout vec4 under) {
    vec4 over = texture(overTex, otherTextureCoordinate(overTex_resolution));

    under = vec4(mix(under.rgb, over.rgb, over.a), clamp(under.a + over.a, 0, 1));
}

#pragma include "../include/footer.glsl"
