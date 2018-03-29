#pragma include "common/frag.glsl"
#pragma include "common/convolution_frag.glsl"

uniform bool invert;

void main() {
    vec3 n[9] = getTextures();

    mat3 kernelX = mat3(
        3, 0, -3,
        10, 0, -10,
        3, 0, -3
    );

    mat3 kernelY = mat3(
        3, 10, 3,
        0, 0, 0,
        -3, -10, -3
    );

    vec3 a = applyKernel(kernelX, n);
    vec3 b = applyKernel(kernelY, n);

	vec3 result = sqrt((a * a) + (b * b));
    if (invert) {
        result = 1. - result;
    }

    outputColor = vec4(result, 1.0);
}