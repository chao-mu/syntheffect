void main() {
    vec3 col = mainFrag();
    vec3 originalCol = texture(tex0, textureCoordinate).rgb;

    if (rangeAdjustEnabled) {
        col /= vec3(rangeAdjustHigh);
    }

    if (greyscaleEnabled) {
        col = vec3(luminance(col));
    }
    
    if (inversionEnabled) {
        if (inversionClamp) {
            col = clamp(col, 0., 1.);
        }

        col = inversionAdjustment - col;
    }

    if (stepEnabled) {
        col = step(stepThreshold, col);
    }

    if (multiplyOriginalEnabled) {
        col = clamp(col, 0., 1.);
        col *= originalCol;
    }

    col = mix(originalCol, col, mixture);

    outputColor = vec4(col, 1.0);
}