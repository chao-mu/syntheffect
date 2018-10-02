#pragma include "../include/header.glsl"

uniform sampler2DRect texMap;

uniform float max_distance = 5;

vec2 fromAtan01(float x) {
    x = 2 * PI * x - PI;
    
    return vec2(cos(x), sin(x));
}

vec4 mainFrag() {
    vec4 info = texture(texMap, textureCoordinate);
    vec2 dir = fromAtan01(info.a);
    
    vec2 uv = normalize_1to1(textureCoordinate, resolution);
    float red = texture(tex0, denormalize_1to1(info.r * dir * max_distance + uv, resolution)).r;
    float green = texture(tex0, denormalize_1to1(info.g  * dir * max_distance + uv, resolution)).g;
    float blue = texture(tex0, denormalize_1to1(info.b * max_distance + uv, resolution)).b;
    float alpha = texture(tex0, textureCoordinate).a;

    return vec4(red, green, blue, alpha);
}

#pragma include "../include/footer.glsl"
