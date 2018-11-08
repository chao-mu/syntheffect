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

/*
float[9] luminance3x3(vec3 n[9]) {
    float lums[9];
    for (int i=0; i < 9; i++) {
        lums[i] = luminance(n[i]);
    }

    return lums;
}
*/

#define _applyKernelSums \
    result += n[6] * kernel[0][0] + n[7] * kernel[0][1] + n[8] * kernel[0][2]; \
    result += n[3] * kernel[1][0] + n[4] * kernel[1][1] + n[5] * kernel[1][2]; \
    result += n[0] * kernel[2][0] + n[1] * kernel[2][1] + n[2] * kernel[2][2];

float applyKernel(mat3 kernel, float[9] n) {
    float result = 0;

    _applyKernelSums(kernel, n);

    return result;
}

vec3 applyKernel(mat3 kernel, vec3[9] n) {
    vec3 result = vec3(0);

    _applyKernelSums(kernel, n);

    return result;
}
