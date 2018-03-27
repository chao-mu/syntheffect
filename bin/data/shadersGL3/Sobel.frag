#version 330

uniform sampler2DRect tex0;

uniform float time;
uniform vec2 resolution;

out vec4 outputColor;

in vec2 texture_coord;
in vec2 leftTextureCoordinate;
in vec2 rightTextureCoordinate;

in vec2 topTextureCoordinate;
in vec2 topLeftTextureCoordinate;
in vec2 topRightTextureCoordinate;

in vec2 bottomTextureCoordinate;
in vec2 bottomLeftTextureCoordinate;
in vec2 bottomRightTextureCoordinate;

vec3[9] getTextures() {
    vec3 n[9];

    n[0] = texture(tex0, bottomLeftTextureCoordinate).rgb;
    n[1] = texture(tex0, bottomTextureCoordinate).rgb;
    n[2] = texture(tex0, bottomRightTextureCoordinate).rgb;

    n[3] = texture(tex0, leftTextureCoordinate).rgb;
    n[4] = texture(tex0, texture_coord).rgb;
    n[5] = texture(tex0, rightTextureCoordinate).rgb;

    n[6] = texture(tex0, topLeftTextureCoordinate).rgb;
    n[7] = texture(tex0, topTextureCoordinate).rgb;
    n[8] = texture(tex0, topRightTextureCoordinate).rgb;

    return n;
}

void main() {
    vec3 n[9] = getTextures();

    // https://gist.github.com/Hebali/6ebfc66106459aacee6a9fac029d0115
	vec3 sobel_edge_h = n[2] + (2.0*n[5]) + n[8] - (n[0] + (2.0*n[3]) + n[6]);
  	vec3 sobel_edge_v = n[0] + (2.0*n[1]) + n[2] - (n[6] + (2.0*n[7]) + n[8]);
	vec3 sobel = sqrt((sobel_edge_h * sobel_edge_h) + (sobel_edge_v * sobel_edge_v));

    outputColor = vec4(sobel.rgb, 1.0);
}