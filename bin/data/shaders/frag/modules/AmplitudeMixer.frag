#pragma include "../include/header.glsl"

DEFINE_INPUT_TEXTURE(classifier, DESC("The classification signals. High is left, low is right"))
DEFINE_INPUT_TEXTURE(right, DESC("The right hand signals to be mixed into the left according to the classifying signal"))

uniform float stepLow = 0;
uniform float stepHigh = 1;
uniform bool smoothEnabled = false;

void mainFrag(inout vec4 left) {
    vec4 classifierSignal = GET_OTHER_TEXTURE(classifier);

    left = mix(left, GET_OTHER_TEXTURE(right), classifierSignal);
}

#pragma include "../include/footer.glsl"
