#pragma include "include/header.glsl"

void mainFrag(inout vec4 col) {
    col.rgb = luminance(col.rgb);
}

#pragma include "include/footer.glsl"
