#version 330

uniform sampler2DRect tex0;

uniform float time;
uniform vec2 resolution;

out vec4 outputColor;

in vec2 texture_coord;
in vec2 leftTextureCoordinate;
in vec2 rightTextureCoordinate;

in vec2 topTextureCoordinate;
in vec2 topLeftTextureCoordinate;
in vec2 topRightTextureCoordinate;

in vec2 bottomTextureCoordinate;
in vec2 bottomLeftTextureCoordinate;
in vec2 bottomRightTextureCoordinate;

vec3[9] getTextures() {
    vec3 n[9];

    n[0] = texture(tex0, bottomLeftTextureCoordinate).rgb;
    n[1] = texture(tex0, bottomTextureCoordinate).rgb;
    n[2] = texture(tex0, bottomRightTextureCoordinate).rgb;

    n[3] = texture(tex0, leftTextureCoordinate).rgb;
    n[4] = texture(tex0, texture_coord).rgb;
    n[5] = texture(tex0, rightTextureCoordinate).rgb;

    n[6] = texture(tex0, topLeftTextureCoordinate).rgb;
    n[7] = texture(tex0, topTextureCoordinate).rgb;
    n[8] = texture(tex0, topRightTextureCoordinate).rgb;

    return n;
}

vec3 applyKernel(mat3 kernel, vec3[9] n) {
    vec3 result = vec3(0);

    result += n[6] * kernel[0][0] + n[7] * kernel[0][1] + n[8] * kernel[0][2];
    result += n[3] * kernel[1][0] + n[4] * kernel[1][1] + n[5] * kernel[1][2];
    result += n[0] * kernel[2][0] + n[1] * kernel[2][1] + n[2] * kernel[2][2];

    return result;
}

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

    outputColor = vec4(result, 1.0);
}