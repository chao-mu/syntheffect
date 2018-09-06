#pragma include "../include/header.glsl"

vec4 mainFrag() {
    vec4 n[9] = get3x3();

    return applyKernel(KERNEL_BOX_BLUR, n) / 9.;
}

#pragma include "../include/footer.glsl"
