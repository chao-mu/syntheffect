#pragma include "../include/header.glsl"

uniform sampler2DRect secondTex;
uniform float threshold = 0.1;

vec3 mainFrag()
{
    vec3 new = texture(tex0, textureCoordinate).rgb;
    vec3 old = texture(secondTex, textureCoordinate).rgb;
    
    if (abs(luminance(new) - luminance(old)) > threshold) {
        return new;
    } else {
        return old;
    }
}

#pragma include "../include/footer.glsl"
