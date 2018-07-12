#pragma include "include/header.glsl"

uniform sampler2DRect displacerTex;
uniform float scale = 1.;

vec3 mainFrag()
{
    vec3 noise = texture(displacerTex, textureCoordinate).rgb;
    
    vec2 uv = normalize_0to1(textureCoordinate, resolution);
    uv += (noise.rg * scale);
    uv = denormalize_0to1(uv, resolution);
 
    return texture(tex0, uv).rgb;
}

#pragma include "include/footer.glsl"
