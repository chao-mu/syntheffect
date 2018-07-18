#pragma include "include/header.glsl"

vec3 mainFrag()
{
    vec3 color = texture(tex0, textureCoordinate).rgb;
    
    vec3 hsv = rgb2hsv(color);
    hsv.x = luminance(color);
 
    return hsv2rgb(hsv);
}

#pragma include "include/footer.glsl"
