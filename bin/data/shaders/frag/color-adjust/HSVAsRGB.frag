#pragma include "../include/header.glsl"

vec3 mainFrag()
{
    vec4 color = texture(tex0, textureCoordinate);
    
    vec3 res = rgb2hsv(color.rgb);

    return res;
}

#pragma include "../include/footer.glsl"
