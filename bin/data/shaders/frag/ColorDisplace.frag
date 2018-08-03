#pragma include "include/header.glsl"

float d = 0.05; 
uniform float speed = 1.;

uniform bool displaceRed = true;
uniform bool displaceGreen = true;
uniform bool displaceBlue = true;
uniform bool displaceInverse = false;
uniform bool displaceRGB = false;

vec4 getNormalized(vec2 uv) {
    return texture(tex0, uv * resolution);
}

float noiseyChannel(vec3 base, vec2 uv, int channel) {
    float t = speed * time;
    uv.x += mix(-d, d, noise(t + ((channel + 1) * 5)));
    uv.y += mix(-d, d, noise(t + ((channel + 1) * 7)));
    return mix(base[channel], getNormalized(uv)[channel], 0.5);
}

vec3 noiseyInverse(vec3 color, vec2 uv) {
    float t = speed * time;
    uv.x += mix(-d, d, noise(t + (4 * 5)));
    uv.y += mix(-d, d, noise(t + (4 * 7)));

    return mix(color, vec3(1.) - getNormalized(uv).rgb, 0.5);
}

vec3 mainFrag() {
    vec2 uv = textureCoordinate / resolution;

    vec3 color = texture(tex0, textureCoordinate).rgb;

    if (displaceRed || displaceRGB) {
        color.r = noiseyChannel(color, uv, 0);
    }
    
    if (displaceGreen || displaceRGB) {
        color.g = noiseyChannel(color, uv, 1);
    }
    
    if (displaceBlue || displaceRGB) {
        color.b = noiseyChannel(color, uv, 2);
    }
    
    if (displaceInverse) {
        color = noiseyInverse(color, uv);
    }

    return color;
}

#pragma include "include/footer.glsl"
