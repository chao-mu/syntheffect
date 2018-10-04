#pragma include "../include/header.glsl"

DEFINE_INPUT_TEXTURE(right, DESC("the signal to mixin"))

uniform float shift = 0;

void mainFrag(inout vec4 col) {
    vec3 leftCol = col.rgb;
    vec3 rightCol = GET_OTHER_TEXTURE(right).rgb;

    col.rgb = sqrt((rightCol * rightCol + shift) + (leftCol * leftCol));
}

#pragma include "../include/footer.glsl"
