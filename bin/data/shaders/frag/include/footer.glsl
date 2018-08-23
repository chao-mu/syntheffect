void main() {
    vec4 originalCol = texture(tex0, textureCoordinate);
    
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
            col *= originalCol.rgb;
        }

        col = mix(originalCol.rgb, col.rgb, mixture);

        outputColor = vec4(col, originalCol.a);
    } else {
        outputColor = originalCol;
    }
}
