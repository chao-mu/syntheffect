#version 330

uniform mat4 modelViewProjectionMatrix;

in vec4 position;
in vec2 texcoord;

out vec2 textureCoordinate;

out vec2 leftTextureCoordinate;
out vec2 rightTextureCoordinate;

out vec2 topTextureCoordinate;
out vec2 topLeftTextureCoordinate;
out vec2 topRightTextureCoordinate;

out vec2 bottomTextureCoordinate;
out vec2 bottomLeftTextureCoordinate;
out vec2 bottomRightTextureCoordinate;

uniform int scale = 1;

void main(){
    gl_Position = modelViewProjectionMatrix * position;

    textureCoordinate = texcoord;
    
    leftTextureCoordinate = texcoord.xy + vec2(-scale, 0);
    rightTextureCoordinate = texcoord.xy + vec2(scale, 0);

    topTextureCoordinate = texcoord.xy + vec2(0, scale);
    topLeftTextureCoordinate = texcoord.xy + vec2(-scale, scale);
    topRightTextureCoordinate = texcoord.xy + vec2(scale, scale);

    bottomTextureCoordinate = texcoord.xy + vec2(0, -scale);
    bottomLeftTextureCoordinate = texcoord.xy + vec2(-scale, -scale);
    bottomRightTextureCoordinate = texcoord.xy + vec2(scale, -scale);
}
