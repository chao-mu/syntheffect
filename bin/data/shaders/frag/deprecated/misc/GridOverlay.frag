#pragma include "../include/header.glsl"

uniform float boxes = 10;
uniform float minimum = 0.5;

vec4 mainFrag() {
    vec2 uv = normalize_0to1(textureCoordinate, resolution);

    vec4 color = texture(tex0, textureCoordinate);
    
    vec2 px = uv * boxes;
    
    // Decide the shade of each panel
    float shade = clamp(0.5 + rand(floor(px + 0.5)), 0., 1.);
    
    // Calculate the grid
    float g = smoothstep(0.4, 0.49, distance(floor(px.x + 0.5), px.x))
        + smoothstep(0.4, 0.49, distance(floor(px.y + 0.5), px.y));
     g = 1. - clamp(g, 0., 1.);
        

     // Apply grid and color;
    return vec4(g * color.rgb * shade, color.a);
}

#pragma include "../include/footer.glsl"
