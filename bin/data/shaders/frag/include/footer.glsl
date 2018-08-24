void main() {
    vec4 originalCol = texture(tex0, textureCoordinate);
    
    if (enabled) {
        vec4 col = mainFrag();

        if (rangeAdjustEnabled) {
            col.rgb /= vec3(rangeAdjustHigh);
        }

        if (greyscaleEnabled) {
            col.rgb = vec3(luminance(col.rgb));
        }
        
        if (inversionEnabled) {
            if (inversionClamp) {
                col.rgb = clamp(col.rgb, 0., 1.);
            }

            col.rgb = inversionAdjustment - col.rgb;
        }

        if (stepEnabled) {
            col.rgb = step(stepThreshold, col.rgb);
        }

        if (multiplyOriginalEnabled) {
            col.rgb = clamp(col.rgb, 0., 1.);
            col.rgb *= originalCol.rgb;
        }

        col = mix(originalCol, col, mixture);

        outputColor = col;
    } else {
        outputColor = originalCol;
    }
}
