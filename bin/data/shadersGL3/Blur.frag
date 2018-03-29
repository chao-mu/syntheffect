#pragma include "common/frag.glsl"
#pragma include "common/convolution_frag.glsl"

vec3 main3x3(vec3 n[9]) {
    return applyKernel(KERNEL_BOX_BLUR, n) / 9.;
}

#pragma include "common/convolution_3x3_main.glsl"