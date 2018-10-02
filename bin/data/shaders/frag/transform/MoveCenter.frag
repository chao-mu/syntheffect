#pragma include "../include/header.glsl"

uniform float x = 0.;
uniform float y = 0.;

void mainFrag(inout vec4 col)
{
    vec2 off = vec2(x, y) * resolution;
    vec2 coords = mod(textureCoordinate + off, resolution);
    
    col = texture(tex0, coords);
}

#pragma include "../include/footer.glsl"
