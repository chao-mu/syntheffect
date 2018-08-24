#pragma include "../include/header.glsl"

vec4 mainFrag()
{
    vec4 color = texture(tex0, textureCoordinate);
    
    vec3 res = rgb2hsv(color.rgb);

    return vec4(res, color.a);
}

#pragma include "../include/footer.glsl"
