#pragma include "include/header.glsl"

vec3 mainFrag()
{
    vec3 color = rgb2hsv(texture(tex0, textureCoordinate).rgb);
    
    return vec3(color.x);
}

#pragma include "include/footer.glsl"
