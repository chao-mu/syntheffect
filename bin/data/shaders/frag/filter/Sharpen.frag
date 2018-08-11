#pragma include "../include/header.glsl"
#pragma include "../include/convolution_3x3.glsl"

vec3 mainFrag() {
    vec3 n[9] = get3x3();

    return applyKernel(KERNEL_SHARPEN, n);
}

#pragma include "../include/footer.glsl"
