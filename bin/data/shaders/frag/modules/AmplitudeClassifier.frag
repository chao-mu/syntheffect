#pragma include "../include/header.glsl"
 
#define INPUT_COUNT 8

#define BLEND_BAND(target_band, next_band) \
    if (target_band == band) { \
        if (input ## target_band ## _passed) { \
            col = GET_OTHER_TEXTURE(input ## target_band); \
        } else { \
            col = GET_OTHER_TEXTURE(inputDefault); \
        } \
        if (input ## next_band ## _passed ) { \
            col = mix(col, GET_OTHER_TEXTURE(input ## next_band), bleed * fract(bandRotation)); \
        } \
    }

uniform float low = 0;
uniform float high = 1;
uniform float bandRotation = 0;
uniform float bleed = 0;

DEFINE_TEXTURE(input0)
DEFINE_TEXTURE(input1)
DEFINE_TEXTURE(input2)
DEFINE_TEXTURE(input3)
DEFINE_TEXTURE(input4)
DEFINE_TEXTURE(input5)
DEFINE_TEXTURE(input6)
DEFINE_TEXTURE(input7)
DEFINE_TEXTURE(inputDefault)

void mainFrag(inout vec4 col) {
    //float amp = texture(amplitudeTex, textureCoordinate).x;
    float amp = get_luminance(col.rgb);
    
    float s = (high - low) / INPUT_COUNT;
        
    int band = 0;
    for (float i=0; i < high; i += s) {
        if (amp >= i && amp < i + s) { 
            break;
        }
        band++;
    }

    band = int(bandRotation + band) % INPUT_COUNT;

    BLEND_BAND(0, 1)
    BLEND_BAND(1, 2)
    BLEND_BAND(2, 3)
    BLEND_BAND(3, 4)
    BLEND_BAND(4, 5)
    BLEND_BAND(5, 6)
    BLEND_BAND(6, 7)
    BLEND_BAND(7, 0)
}

#pragma include "../include/footer.glsl"
