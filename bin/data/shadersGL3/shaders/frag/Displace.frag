#pragma include "include/header.glsl"

uniform sampler2DRect displacerTex;

vec3 mainFrag()
{
    vec3 noise = texture(displacerTex, textureCoordinate).rgb;
    
    vec2 uv = normalize_0to1(textureCoordinate, resolution);
    //uv = clamp(uv + noise.rb - .2, 0., 1.);
    uv += noise.rb * 0.99;
 
    //uv /= noise.rb;

    vec3 color = texture(tex0, denormalize_0to1(uv, resolution)).rgb;

    return color;
}

#pragma include "include/footer.glsl"