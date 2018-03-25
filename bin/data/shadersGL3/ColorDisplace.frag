#version 330

uniform sampler2DRect tex0;

uniform float time;
uniform vec2 resolution;

out vec4 outputColor;

in vec2 texture_coord;

// Book of Shaders
float rand(float n) {
    return fract(sin(n) * 43758.5453123);
}

float noise(float p, float high) {
	float fl = floor(p);
    float fc = fract(p);

	return mix(0, high, mix(rand(fl), rand(fl + 1.0), fc));
}

// From the book of shaders
mat2 rotate2d(float angle)
{
    return mat2(cos(angle),-sin(angle),
                sin(angle),cos(angle));
}

vec4 getNormalized(vec2 uv) {
    return texture(tex0, uv * resolution);
}

void main() {
    vec2 uv = texture_coord / resolution;
    float intensity = 0.5;

    vec4 color = texture(tex0, uv * resolution);

    vec2 uvRed = uv;
    uvRed.x -= noise(time * (1.0 + sin(time)) * 0.5, 0.05);
    uvRed.y += noise(time * (1.0 + sin(time)) * 0.5, 0.05);
    color.r = mix(color.r, getNormalized(uvRed).r, intensity);

    vec2 uvGreen = uv;
    uvGreen.x += noise(time * (1.0 + sin(time)) * 0.5, 0.05);
    uvGreen.y -= noise(time * (1.0 + sin(time)) * 0.5, 0.05);
    color.g = mix(color.g, getNormalized(uvGreen).g, intensity);

    outputColor = color;
}