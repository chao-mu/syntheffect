#pragma include "../include/header.glsl"

uniform float outOfBoundsR = 0.;
uniform float outOfBoundsG = 0.;
uniform float outOfBoundsB = 0.;

vec4 mainFrag() {
    vec2 uv = textureCoordinate;
    if (uv.x > resolution.x || uv.y > resolution.y || uv.x < 0 || uv.y < 0) {
        return vec3(outOfBoundsR, outOfBoundsG, outOfBoundsB);
    }

    return texture(tex0, uv);
}

#pragma include "../include/footer.glsl"
