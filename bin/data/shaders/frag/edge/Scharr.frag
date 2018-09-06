#pragma include "../include/header.glsl"

vec4 mainFrag() {
    vec4 n[9] = get3x3();

    vec4 a = applyKernel(KERNEL_SCHARR_X, n);
    vec4 b = applyKernel(KERNEL_SCHARR_Y, n);

	return sqrt((a * a) + (b * b));
}

#pragma include "../include/footer.glsl"
