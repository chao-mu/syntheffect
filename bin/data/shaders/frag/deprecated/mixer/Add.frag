#pragma include "../include/header.glsl"

uniform sampler2DRect right;

uniform bool addAlpha = false;
uniform bool addRGB = true;

void mainFrag(inout vec4 leftCol) {
    vec4 rightCol = texture(right, textureCoordinate);

    if (addAlpha) {
        leftCol.a += rightCol.a;
    }

    if (addRGB) {
        leftCol.rgb += rightCol.rgb;
    }
}

#pragma include "../include/footer.glsl"
