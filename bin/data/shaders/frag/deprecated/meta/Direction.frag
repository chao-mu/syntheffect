#pragma include "../include/header.glsl"

uniform sampler2DRect lastTex;

float atan01(float x, float y) {
    return (PI + atan(x, y)) / PI;
}

vec4 mainFrag() {
    vec4 current = texture(tex0, textureCoordinate);
    vec4 diffs[9];
    
    float s = 1;

    // Bottom texture differences
    diffs[0] = vec4(abs(current.rgb - texture(lastTex, bottomLeftTextureCoordinate).rgb), atan01(-s, -s));
    diffs[1] = vec4(abs(current.rgb - texture(lastTex, bottomTextureCoordinate).rgb), atan01(0., -s));
    diffs[2] = vec4(abs(current.rgb - texture(lastTex, bottomRightTextureCoordinate).rgb), atan01(s, -s));

    // Middle row (left to right textures)
    diffs[3] = vec4(abs(current.rgb - texture(lastTex, leftTextureCoordinate).rgb), atan01(-s, 0.));
    diffs[4] = vec4(abs(current.rgb - texture(lastTex, textureCoordinate).rgb), atan01(0., 0.));
    diffs[5] = vec4(abs(current.rgb - texture(lastTex, rightTextureCoordinate).rgb), atan01(s, 0.));

    // Top row, left to right.
    diffs[6] = vec4(abs(current.rgb - texture(lastTex, topLeftTextureCoordinate).rgb), atan01(-s, s));
    diffs[7] = vec4(abs(current.rgb - texture(lastTex, topTextureCoordinate).rgb), atan01(0., s));
    diffs[8] = vec4(abs(current.rgb - texture(lastTex, topRightTextureCoordinate).rgb), atan01(s, s));

    vec4 m = diffs[0];
    float intensity = luminance(m.rgb);
    for (int i=1; i < 9; i++) {
        float newIntensity = luminance(diffs[i].rgb);
        if (newIntensity < intensity) {
            m = diffs[i]; 
            intensity = newIntensity;
        }
    }
    
    return m;
}

#pragma include "../include/footer.glsl"
