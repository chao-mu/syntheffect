#pragma include "include/header.glsl"

uniform sampler2DRect displacerTex;
uniform float scale = 1.;

vec3 mainFrag()
{
    vec3 noise = texture(displacerTex, textureCoordinate).rgb;
    
    vec2 uv = normalize_1to1(textureCoordinate, resolution);
    //uv = clamp(uv + noise.rb - .2, 0., 1.);
    uv += normalize_1to1(noise.rg, vec2(0, 1)) * scale;
 
    //uv /= noise.rb;

    vec3 color = texture(tex0, denormalize_1to1(uv, resolution)).rgb;

    return color;
}

#pragma include "include/footer.glsl"