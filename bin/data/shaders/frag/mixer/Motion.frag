#pragma include "../include/header.glsl"

uniform sampler2DRect last;
uniform float threshold = 0.1;

void mainFrag(inout vec4 left)
{
    vec4 new = texture(tex0, textureCoordinate);
    vec4 old = texture(last, textureCoordinate);

    if (abs(luminance(new.rgb) - luminance(old.rgb)) > threshold) {
        left = new;
    } else {
        left = old;
    }
}

#pragma include "../include/footer.glsl"
