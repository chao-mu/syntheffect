#version 330

uniform sampler2DRect tex0;

uniform float time;
uniform vec2 resolution;

out vec4 outputColor;

in vec2 texture_coord;

float distance = 0.05;

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

vec4 getNormalized(vec2 uv) {
    return texture(tex0, uv * resolution);
}

float noiseyChannel(vec4 base, vec2 uv, int channel) {
    uv.x += mix(-distance, distance, noise(time + ((channel + 1) * 5)));
    uv.y += mix(-distance, distance, noise(time + ((channel + 1) * 7)));
    return mix(base[channel], getNormalized(uv)[channel], 0.5);
}

void main() {
    vec2 uv = texture_coord / resolution;

    vec4 color = texture(tex0, uv * resolution);

    color.r = noiseyChannel(color, uv, 0);
    color.g = noiseyChannel(color, uv, 1);
    color.b = noiseyChannel(color, uv, 2);

    outputColor = color;
}