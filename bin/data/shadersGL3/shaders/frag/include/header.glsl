#version 330

uniform sampler2DRect tex0;

uniform float time;
uniform vec2 resolution;

uniform bool greyscaleEnabled = false;

uniform bool inversionEnabled = false;
uniform bool inversionClamp = false;
uniform float inversionAdjustment = 1.;

uniform bool stepEnabled = false;
uniform float stepThreshold = 0.5;

uniform bool multiplyOriginalEnabled = false;

out vec4 outputColor;

in vec2 textureCoordinate;

// Book of Shaders
float rand(float n) {
    return fract(sin(n) * 43758.5453123);
}

// Book of Shaders
float noise(float p) {
	float fl = floor(p);
    float fc = fract(p);

	return mix(rand(fl), rand(fl + 1.0), fc);
}

vec3 rgb2hsv(vec3 c)
{
    vec4 K = vec4(0.0, -1.0 / 3.0, 2.0 / 3.0, -1.0);
    vec4 p = mix(vec4(c.bg, K.wz), vec4(c.gb, K.xy), step(c.b, c.g));
    vec4 q = mix(vec4(p.xyw, c.r), vec4(c.r, p.yzx), step(p.x, c.r));

    float d = q.x - min(q.w, q.y);
    float e = 1.0e-10;

    return vec3(abs(q.z + (q.w - q.y) / (6.0 * d + e)), d / (q.x + e), q.x);
}

vec3 hsv2rgb(vec3 c)
{
    vec4 K = vec4(1.0, 2.0 / 3.0, 1.0 / 3.0, 3.0);
    vec3 p = abs(fract(c.xxx + K.xyz) * 6.0 - K.www);

    return c.z * mix(K.xxx, clamp(p - K.xxx, 0.0, 1.0), c.y);
}

// From the book of shaders
mat2 rotate2d(float angle)
{
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