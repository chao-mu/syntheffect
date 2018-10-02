#pragma include "../include/header.glsl"

void mainFrag(inout vec4 col) {
    vec4 n[9] = get3x3();

    vec3 a = applyKernel(KERNEL_SCHARR_X, n).rgb;
    vec3 b = applyKernel(KERNEL_SCHARR_Y, n).rgb;

	col = vec4(sqrt((a * a) + (b * b)), n[4].a);
}

#pragma include "../include/footer.glsl"
