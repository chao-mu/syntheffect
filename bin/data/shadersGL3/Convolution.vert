#version 330

uniform mat4 modelViewProjectionMatrix;

in vec4 position;
in vec2 texcoord;

out vec2 texture_coord;

out vec2 leftTextureCoordinate;
out vec2 rightTextureCoordinate;

out vec2 topTextureCoordinate;
out vec2 topLeftTextureCoordinate;
out vec2 topRightTextureCoordinate;

out vec2 bottomTextureCoordinate;
out vec2 bottomLeftTextureCoordinate;
out vec2 bottomRightTextureCoordinate;

void main(){
    gl_Position = modelViewProjectionMatrix * position;

    texture_coord = texcoord;

    int scale = 1;

    leftTextureCoordinate = texture_coord.xy + vec2(-scale, 0);
    rightTextureCoordinate = texture_coord.xy + vec2(scale, 0);

    topTextureCoordinate = texture_coord.xy + vec2(0, scale);
    topLeftTextureCoordinate = texture_coord.xy + vec2(-scale, scale);
    topRightTextureCoordinate = texture_coord.xy + vec2(scale, scale);

    bottomTextureCoordinate = texture_coord.xy + vec2(0, -scale);
    bottomLeftTextureCoordinate = texture_coord.xy + vec2(-scale, -scale);
    bottomRightTextureCoordinate = texture_coord.xy + vec2(scale, -scale);
}