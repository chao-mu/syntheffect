#version 330

#pragma include "../../3rdparty/ColorSpaces.inc.glsl"

#define PI 3.14159265359
#define PI_HALF 1.57079632679

uniform sampler2DRect tex0;

uniform float time;
uniform vec2 resolution;

uniform bool enabled = true;

uniform float mixture = 1.;
uniform float channel1Mix = 1.;
uniform float channel2Mix = 1.;
uniform float channel3Mix = 1.;
uniform float channel4Mix = 1.;
uniform bool invert = false;

uniform sampler2DRect maskTex;
uniform vec2 maskTex_resolution;
uniform bool maskTex_passed = false;
uniform bool maskInvert = false;
uniform int maskChannel;

out vec4 outputColor;

in vec2 textureCoordinate;

#define DESC(s) #s

#define DEFINE_OUTPUT_CHANNEL(num, name, desc)

#define GET_OTHER_TEXTURE(texName) \
    texture(texName ## , otherTextureCoordinate(texName ## _resolution))

#define DEFINE_INPUT_TEXTURE(name, desc) \
    uniform sampler2DRect name; \
    uniform bool name ## _passed; \
    uniform vec2 name ## _resolution;

#define DEFINE_INPUT_TEXTURE_CHANNEL(texName, name, i, def, desc) \
    float get_ ## texName ## _ ## name ## (vec4 channels) { \
        if (!texName ## _passed) { \
            return def; \
        } \
        return channels[i]; \
    }


// From the book of shaders
mat2 rotate2d(float angle) {
    return mat2(cos(angle),-sin(angle),
                sin(angle),cos(angle));
}

float luminance(vec3 rgb) {
    return dot(rgb, vec3(0.3, 0.59, 0.11));
}

// translate texture coordinates to -1 to 1.
vec2 normalize_1to1(vec2 coords, vec2 res) {
    return (2. * coords - res.xy) / res.y;
}

// translate coordinates in range -1 to 1 to texture coordinates.
vec2 denormalize_1to1(vec2 uv, vec2 res) {
    return ((uv * res.y) + res.xy) / 2.;
}

// translate texture coordinates to 0 to 1.
vec2 normalize_0to1(vec2 coords, vec2 res) {
    return coords / res;
}

// translate coordinates in range 0 to 1 to texture coordinates.
vec2 denormalize_0to1(vec2 uv, vec2 res) {
    return uv * res;
}

vec2 otherTextureCoordinate(vec2 otherResolution) {
    return vec2(
            (textureCoordinate.x * otherResolution.x) / resolution.x,
            (textureCoordinate.y * otherResolution.y) / resolution.y);
}

// Multiple the result of this function call to rotate the coordinates by the given angle.
#define rotate(angle) mat2(cos(angle),-sin(angle), sin(angle),cos(angle))

/*
// Get current texture coordinate
vec2 tex_coord_0to1() {
    return normalize_0to1(texture_coord, resolution);
}

// translate coordinates in range 0 to 1 to texture coordinates, using global resolution
vec2 denormalize_0to1(vec2 uv) {
    return denormalize_0to1(uv, resolution.xy);
}

// translate coordinates in range -1 to 1 to texture coordinates.
vec2 denormalize_1to1(vec2 uv) {
    return ((uv * resolution.y) + resolution.xy) / 2.
}

// Produce a 1 if the provided value is <= 0, with a small transitional blur
#define S(v) smoothstep(blur, 0., v)

// Draw (return 1) a circle at the origin with the radius of r.
#define circle(U, r) S(length(U) - r)

// Draw (return 1) a crescent made from two overlapping circles.
// The overlap is controlled by the provided offset. The two radiuses are determined by r.
#define crescent(U, offset, r) circle(U, r) - circle( U - vec2(offset,0), r)

// Draw a line segment of length and width.
#define segment(U, len, width)  S( abs(U.x)-width ) * S( abs(U.y)-len )

*/

#pragma include "noise.glsl"
#pragma include "convolution_3x3.glsl"
