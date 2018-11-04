#pragma include "../include/header.glsl"

void mainFrag(inout vec4 col) {
    vec4 n[9] = get3x3();

    col = applyKernel(KERNEL_BOX_BLUR, n) / 9;
}

#pragma include "../include/footer.glsl"
