#pragma include "../include/header.glsl"

vec3 mainFrag()
{
    vec4 color = texture(tex0, textureCoordinate);
    
    vec3 hsv = rgb2hsv(color.rgb);
    hsv.x = hsv.z;
    hsv.y = 1.0;
    hsv.z = 1.0;
 
    return hsv2rgb(hsv);
}

#pragma include "../include/footer.glsl"
