void main() {
    vec3 col = mainFrag();
    
    if (inversionEnabled) {
        if (inversionClamp) {
            col = clamp(col, 0., 1.);
        }

        col = inversionAdjustment - col;
    }

    if (greyscaleEnabled) {
        col = vec3(luminance(col));
    }

    if (stepEnabled) {
        col = step(stepThreshold, col);
    }

    if (multiplyOriginalEnabled) {
        vec3 originalCol = texture(tex0, textureCoordinate).rgb;
        col = clamp(col, 0., 1.);
        col *= originalCol;
    }

    outputColor = vec4(col, 1.0);
}