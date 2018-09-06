#define KERNEL_SCHARR_Y \
    mat3( \
        3, 10, 3, \
        0, 0, 0, \
        -3, -10, -3 \
    )

#define KERNEL_SCHARR_X \
    mat3( \
        3, 0, -3, \
        10, 0, -10, \
        3, 0, -3 \
    )

#define KERNEL_BOX_BLUR \
    mat3( \
        1, 1, 1, \
        1, 1, 1, \
        1, 1, 1 \
    )

#define KERNEL_SHARPEN \
    mat3( \
        0, -1, 0, \
        -1, 5, -1, \
        0, -1, 0 \
    )

#define DEFINE_GET_3X3(func_name, tex) \
    vec4[9] ## func_name ## () { \
        vec4 n[9]; \
        n[0] = texture( ## tex ## , bottomLeftTextureCoordinate); \
        n[1] = texture( ## tex ## , bottomTextureCoordinate); \
        n[2] = texture( ## tex ## , bottomRightTextureCoordinate); \
        n[3] = texture( ## tex ## , leftTextureCoordinate); \
        n[4] = texture( ## tex ## , textureCoordinate); \
        n[5] = texture( ## tex ## , rightTextureCoordinate); \
        n[6] = texture( ## tex ## , topLeftTextureCoordinate); \
        n[7] = texture( ## tex ## , topTextureCoordinate); \
        n[8] = texture( ## tex ## , topRightTextureCoordinate); \
        return n; \
    }
 
in vec2 leftTextureCoordinate;
in vec2 rightTextureCoordinate;

in vec2 topTextureCoordinate;
in vec2 topLeftTextureCoordinate;
in vec2 topRightTextureCoordinate;

in vec2 bottomTextureCoordinate;
in vec2 bottomLeftTextureCoordinate;
in vec2 bottomRightTextureCoordinate;

DEFINE_GET_3X3(get3x3, tex0)

vec3[9] rgb3x3(vec4 n[9]) {
    vec3 rgb[9];
    for (int i=0; i < 9; i++) {
        rgb[i] = n[i].rgb;
    }

    return rgb;
}

float[9] luminance3x3(vec4 n[9]) {
    float lums[9];
    for (int i=0; i < 9; i++) {
        lums[i] = luminance(n[i].rgb);
    }

    return lums;
}

#define _applyKernelSums \
    result += n[6] * kernel[0][0] + n[7] * kernel[0][1] + n[8] * kernel[0][2]; \
    result += n[3] * kernel[1][0] + n[4] * kernel[1][1] + n[5] * kernel[1][2]; \
    result += n[0] * kernel[2][0] + n[1] * kernel[2][1] + n[2] * kernel[2][2];

float applyKernel(mat3 kernel, float[9] n) {
    float result = 0;

    _applyKernelSums(kernel, n);

    return result;
}

vec4 applyKernel(mat3 kernel, vec4[9] n) {
    vec4 result = vec4(0);
    
    _applyKernelSums(kernel, n);

    return result;
}
