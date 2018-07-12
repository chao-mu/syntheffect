#version 330

uniform mat4 modelViewProjectionMatrix;

uniform float xSkew = 0.;
uniform float ySkew = 0.;

in vec4 position;
in vec2 texcoord;

out vec2 textureCoordinate;

void main() {
    gl_Position = modelViewProjectionMatrix * position;

    mat3 trans = mat3(
        1., tan(xSkew), 0.,
        tan(ySkew), 1., 0.,
        0., 0., 1.
    );

    textureCoordinate = (trans * (vec3(texcoord.xy, 0.))).xy;
}
