#pragma include "../include/header.glsl"

void mainFrag(inout vec4 col) {
    vec3 yuv = rgb2yuv(col.rgb);
    yuv.z += yuv.y * 0.5;
    yuv.y = 0.0;
    
    col.rgb = yuv2rgb(yuv);
}

#pragma include "../include/footer.glsl"
