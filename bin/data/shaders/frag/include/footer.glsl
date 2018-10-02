void main() {
    vec4 originalCol = texture(tex0, textureCoordinate);
    
    if (!enabled) {
        outputColor = originalCol;
        return;
    }
    
    vec4 col = originalCol;
    
    mainFrag(col);

    col = mix(originalCol, col, vec4(channel1Mix, channel2Mix, channel3Mix, channel4Mix));
    
    if (maskTex_passed) {
        float m = texture(maskTex, otherTextureCoordinate(maskTex_resolution))[maskChannel]; 
        if (maskInvert) {
            m = 1. - m;
        }
        
        col = mix(originalCol, col, m);
    }

    col = mix(originalCol, col, mixture);
    
    if (invert) {
        col = vec4(1.) - col;
    }

    outputColor = col;
}
