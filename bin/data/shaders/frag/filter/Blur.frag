#pragma include "../include/header.glsl"
#pragma include "../include/convolution_3x3.glsl"

vec4 mainFrag() {
    vec3 n[9] = get3x3();

    return vec4(applyKernel(KERNEL_BOX_BLUR, n) / 9., n[4].a);
}

#pragma include "../include/footer.glsl"
