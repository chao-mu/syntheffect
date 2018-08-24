#pragma include "../include/header.glsl"

uniform float x = 0.;
uniform float y = 0.;

vec4 mainFrag()
{
    vec2 off = vec2(x, y) * resolution;
    vec2 coords = mod(textureCoordinate + off, resolution);
    
    return texture(tex0, coords);
}

#pragma include "../include/footer.glsl"
