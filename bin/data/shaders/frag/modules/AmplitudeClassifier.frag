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

DEFINE_INPUT_TEXTURE(input0, DESC("input 1"))
DEFINE_INPUT_TEXTURE(input1, DESC("input 2"))
DEFINE_INPUT_TEXTURE(input2, DESC("input 3"))
DEFINE_INPUT_TEXTURE(input3, DESC("input 4"))
DEFINE_INPUT_TEXTURE(input4, DESC("input 5"))
DEFINE_INPUT_TEXTURE(input5, DESC("input 6"))
DEFINE_INPUT_TEXTURE(input6, DESC("input 7"))
DEFINE_INPUT_TEXTURE(input7, DESC("input 8"))
DEFINE_INPUT_TEXTURE(inputDefault, DESC("input chosen when input not defined"))

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
