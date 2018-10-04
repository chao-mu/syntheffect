#pragma include "../include/header.glsl"

void main()
{
    vec4 color = texture(tex0, textureCoordinate);
    vec3 res = rgb_to_hsv(color.rgb);

    outputColor = vec4(res, color.a);
}
