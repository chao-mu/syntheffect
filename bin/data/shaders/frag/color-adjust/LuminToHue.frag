#pragma include "../include/header.glsl"

vec4 mainFrag()
{
    vec4 color = texture(tex0, textureCoordinate);
    
    vec3 hsv = rgb2hsv(color);
    hsv.x = luminance(color);
 
    return vec4(hsv2rgb(hsv), color.a);
}

#pragma include "../include/footer.glsl"
