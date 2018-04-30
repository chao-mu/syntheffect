#pragma include "include/header.glsl"

float mixture(float freq, float offset) {
    return (1. + sin(time * freq + offset)) * 0.5;
}

vec3 mainFrag()
{
    vec4 color = texture(tex0, textureCoordinate);
    
    vec3 res = rgb2hsv(color.rgb);

    res.r = mix(res.r, color.r, mixture(1, 0));
    res.g = mix(res.g, color.g, mixture(2, 0));
    res.b = mix(res.b, color.b, mixture(1.5, 0));

    return res;
}

#pragma include "include/footer.glsl"