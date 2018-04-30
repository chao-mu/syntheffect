#pragma include "include/header.glsl"

uniform sampler2DRect secondTex;

vec3 mainFrag()
{
    vec4 a = texture(tex0, textureCoordinate);
    vec4 b = texture(secondTex, textureCoordinate);

    return abs(a - b).rgb;
}

#pragma include "include/footer.glsl"