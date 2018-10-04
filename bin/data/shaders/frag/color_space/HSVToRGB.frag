#pragma include "../include/header.glsl"

void main()
{
    vec4 color = texture(tex0, textureCoordinate);
    vec3 res = hsv_to_rgb(color.rgb);

    outputColor = vec4(res, color.a);
}
