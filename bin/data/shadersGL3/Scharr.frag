#pragma include "common/frag.glsl"
#pragma include "common/convolution_frag.glsl"

vec3 main3x3(vec3 n[9]) {
    vec3 a = applyKernel(KERNEL_SCHARR_X, n);
    vec3 b = applyKernel(KERNEL_SCHARR_Y, n);

	return sqrt((a * a) + (b * b));
}

#pragma include "common/convolution_3x3_main.glsl"