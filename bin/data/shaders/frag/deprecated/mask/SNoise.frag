#pragma include "../include/header.glsl"

uniform float scale = 1.;

vec4 mainFrag() {
    vec2 uv = normalize_1to1(textureCoordinate, resolution);

    vec2 st = uv * 10. * scale;
    st = vec2(snoise(st));
    st += time;

    return vec4(snoise(st) * .5 + .5, 1);
}

#pragma include "../include/footer.glsl"
