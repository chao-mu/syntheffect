#pragma include "../include/header.glsl"

vec4 mainFrag() {
    vec3 n[9] = get3x3();

    vec3 a = applyKernel(KERNEL_SCHARR_X, n);
    vec3 b = applyKernel(KERNEL_SCHARR_Y, n);

	return vec4(sqrt((a * a) + (b * b)), 1.);
}

#pragma include "../include/footer.glsl"
