#pragma include "../include/header.glsl"

uniform sampler2DRect secondTex;

uniform float spokes = 6.;
uniform float speed = 1.;
uniform float anchorX = 0.5;
uniform float anchorY = 0.5;

vec3 mainFrag() {
    vec2 uv = normalize_0to1(textureCoordinate, resolution);
    
    float theta = atan(uv.y - anchorX, uv.x - anchorY);
    float percent = theta / (2.0*3.14159);
    vec4 color = vec4(0.);
    if (mod(percent * spokes + speed * time, 2.) < 1.) {
        color = texture(tex0, denormalize_0to1(uv, resolution));
    } else {
        color = texture(secondTex, denormalize_0to1(uv, resolution));
    }

    return color.rgb;
}

#pragma include "../include/footer.glsl"
