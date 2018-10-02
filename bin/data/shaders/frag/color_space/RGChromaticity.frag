#pragma include "../include/header.glsl"

void mainFrag(inout vec4 col) {
    float sum = col.r + col.g + col.b;
    col.r = col.r / sum;
    col.g = col.g / sum;
    col.b = col.b / sum;
}

#pragma include "../include/footer.glsl"
