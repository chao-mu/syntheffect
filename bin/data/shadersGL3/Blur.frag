#pragma include "common/frag.glsl"
#pragma include "common/convolution_frag.glsl"

#define main3x3(n) applyKernel(KERNEL_BOX_BLUR, n) / 9.

#pragma include "common/convolution_3x3_main.glsl"