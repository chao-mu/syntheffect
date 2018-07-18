void main() {
    vec3 originalCol = texture(tex0, textureCoordinate).rgb;
    
    if (enabled) {
        vec3 col = mainFrag();

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
    } else {
        outputColor = vec4(originalCol, 1.0);
    }
}
