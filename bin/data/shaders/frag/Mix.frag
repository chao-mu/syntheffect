#pragma include "include/header.glsl"

uniform sampler2DRect secondTex;
uniform float amount = 0.5;

uniform bool redEnabled = true;
uniform bool greenEnabled = true;
uniform bool blueEnabled = true;

// 0 = Linear interpolation (GLSL mix functon)
// 1 = square root of added squares.
uniform int method = 0;

#define METHOD_LINEAR_INTERP 0
#define METHOD_SQUARE_SUMS 1
#define METHOD_ADD 2
#define METHOD_MULTIPLY 3

// Mix right into left
float mixWithMethod(float left, float right, float amount) {
    if (method == METHOD_LINEAR_INTERP) {
        return mix(left, right, amount);
    }

    if (method == METHOD_SQUARE_SUMS) {
        return sqrt(mix(left * left, right * right, amount));
    }

    if (method == METHOD_ADD) {
        return left + right;
    }

    if (method == METHOD_MULTIPLY) {
        return left * right;
    }


    return 0;
}

vec3 mainFrag()
{
    vec3 left = texture(tex0, textureCoordinate).rgb;
    vec3 right = texture(secondTex, textureCoordinate).rgb;

    if (redEnabled) {
        left.r = mixWithMethod(left.r, right.r, amount);
    }

    if (greenEnabled) {
        left.g = mixWithMethod(left.g, right.g, amount);
    }

    if (blueEnabled) {
        left.b = mixWithMethod(left.b, right.b, amount);
    }

    return left;
}

#pragma include "include/footer.glsl"