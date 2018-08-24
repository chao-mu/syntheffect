#pragma include "../include/header.glsl"

uniform sampler2DRect secondTex;
uniform float threshold = 0.1;

vec4 mainFrag()
{
    vec4 new = texture(tex0, textureCoordinate);
    vec4 old = texture(secondTex, textureCoordinate);
    
    if (abs(luminance(new.rgb) - luminance(old.rgb)) > threshold) {
        return new;
    } else {
        return old;
    }
}

#pragma include "../include/footer.glsl"
