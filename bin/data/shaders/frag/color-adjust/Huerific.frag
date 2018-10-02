#pragma include "../include/header.glsl"

void mainFrag(inout vec4 col) {
    vec3 hsv = rgb2hsv(col.rgb);
    hsv.x = hsv.z;
    hsv.y = 1.;
    hsv.z = 1.;
 
    col.rgb = hsv2rgb(hsv);
}

#pragma include "../include/footer.glsl"
