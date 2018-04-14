#pragma include "include/header.glsl"

uniform sampler2DRect secondTex;
uniform float mixture = 0.5;

uniform bool redEnabled = true;
uniform bool greenEnabled = true;
uniform bool blueEnabled = true;

vec3 mainFrag()
{
    vec3 left = texture(tex0, textureCoordinate).rgb;
    vec3 right = texture(secondTex, textureCoordinate).rgb;

    if (redEnabled) {
        left.r = mix(left.r, right.r, mixture);
    }

    if (greenEnabled) {
        left.g = mix(left.g, right.g, mixture);
    }

    if (blueEnabled) {
        left.b = mix(left.b, right.b, mixture);
    }

    return left;
}

#pragma include "include/footer.glsl"