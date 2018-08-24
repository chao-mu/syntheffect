#pragma include "../include/header.glsl"

uniform sampler2DRect secondTex;

vec4 mainFrag()
{
    vec4 a = texture(tex0, textureCoordinate);
    vec4 b = texture(secondTex, textureCoordinate);

    return vec3(abs(a - b).rgb, a.a);
}

#pragma include "../include/footer.glsl"
