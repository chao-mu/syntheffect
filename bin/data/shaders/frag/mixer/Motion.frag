#pragma include "../include/header.glsl"

uniform sampler2DRect secondTex;
uniform float threshold = 0.1;

void mainFrag(inout vec4 col)
{
    vec4 new = texture(tex0, textureCoordinate);
    vec4 old = texture(secondTex, textureCoordinate);
    
    if (abs(luminance(new.rgb) - luminance(old.rgb)) > threshold) {
        col = new;
    } else {
        col = old;
    }
}

#pragma include "../include/footer.glsl"
