#pragma include "include/header.glsl"
#pragma include "include/convolution_3x3.glsl"

vec3 mainFrag() {
    vec3 n[9] = get3x3();

    vec3 a = applyKernel(KERNEL_SCHARR_X, n);
    vec3 b = applyKernel(KERNEL_SCHARR_Y, n);

	return sqrt((a * a) + (b * b));
}

#pragma include "include/footer.glsl"