#pragma include "../include/header.glsl"

vec4 mainFrag()
{
    vec4 color = rgb2hsv(texture(tex0, textureCoordinate));
    
    return vec4(color.x, color.x, color.x, color.a);
}

#pragma include "../include/footer.glsl"
