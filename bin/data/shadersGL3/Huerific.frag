#pragma include "common/frag.glsl"

void main()
{
    vec4 color = texture(tex0, texture_coord);
    
    vec3 hsv = rgb2hsv(color.rgb);
    hsv.x = hsv.z;
    hsv.y = 1.0;
    hsv.z = 1.0;
    
    vec3 rgb = hsv2rgb(hsv);
 
    outputColor = vec4(rgb, color.w);
}