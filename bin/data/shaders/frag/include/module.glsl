#version 330

#define PI 3.14159265359
#define PI_HALF 1.5707963

#define DESC(s) #s

#define NO_DEFAULT 0.0

#define DEFINE_INPUT(name, def, desc) \
    uniform int name ## TexIdx; \
    uniform int name ## ChannelIdx; \
    uniform bool name ## PropertyPassed; \
    uniform float name ## PropertyValue; \
    uniform float name ## _multiplier; \
    uniform float name ## _shift; \
    \
    float input_ ## name ## (vec2 coord) { \
        float v = def; \
        if (name ## PropertyPassed) { \
            v = name ## PropertyValue; \
        } else if (name ## TexIdx == 0) { \
            v = texture(inputs0, coord)[name ## ChannelIdx]; \
        } else if (name ## TexIdx == 1) { \
            v = texture(inputs1, coord)[name ## ChannelIdx]; \
        } else if (name ## TexIdx == 2) { \
            v = texture(inputs2, coord)[name ## ChannelIdx]; \
        } else if (name ## TexIdx == 3) { \
            v = texture(inputs3, coord)[name ## ChannelIdx]; \
        } else if (name ## TexIdx == 4) { \
            v = texture(inputs4, coord)[name ## ChannelIdx]; \
        } else if (name ## TexIdx == 5) { \
            v = texture(inputs5, coord)[name ## ChannelIdx]; \
        } else if (name ## TexIdx == 6) { \
            v = texture(inputs6, coord)[name ## ChannelIdx]; \
        } else if (name ## TexIdx == 7) { \
            v = texture(inputs7, coord)[name ## ChannelIdx]; \
        } else if (name ## TexIdx == 8) { \
            v = texture(inputs8, coord)[name ## ChannelIdx]; \
        } else if (name ## TexIdx == 9) { \
            v = texture(inputs9, coord)[name ## ChannelIdx]; \
        } \
        \
        return v * name ## _multiplier + name ## _shift; \
    } \
    \
    float input_ ## name ## () { \
        return input_ ## name ## (textureCoordinate); \
    } \
    bool passed_ ## name ## () { \
        return name ## PropertyPassed || name ## TexIdx >= 0; \
    }

#define _DEFINE_OUTPUT(tex_idx, channel_idx, name, desc) \
    void output_ ## name ## (float x) { \
        output ## tex_idx ## [channel_idx] = x; \
        output ## tex_idx ## [3] = 1.0; \
    } \
    float last_output_ ## name(vec2 coords) { \
        return texture(lastOutput ## tex_idx, coords)[channel_idx]; \
    } \
    float last_output_ ## name() { \
        return last_output_ ## name(textureCoordinate); \
    }

#define _DEFINE_OUTPUT_FIRST(tex_idx, channel_idx, name, desc) \
    uniform sampler2DRect lastOutput ## tex_idx; \
    layout (location = tex_idx) out vec4 output ## tex_idx; \
    _DEFINE_OUTPUT(tex_idx, channel_idx, name, desc)

#define DEFINE_OUTPUT_1(name, desc) _DEFINE_OUTPUT_FIRST(0, 0, name, desc)
#define DEFINE_OUTPUT_2(name, desc) _DEFINE_OUTPUT(0, 1, name, desc)
#define DEFINE_OUTPUT_3(name, desc) _DEFINE_OUTPUT(0, 2, name, desc)

#define DEFINE_OUTPUT_4(name, desc) _DEFINE_OUTPUT_FIRST(1, 0, name, desc)
#define DEFINE_OUTPUT_5(name, desc) _DEFINE_OUTPUT(1, 1, name, desc)
#define DEFINE_OUTPUT_6(name, desc) _DEFINE_OUTPUT(1, 2, name, desc)

#define DEFINE_OUTPUT_7(name, desc) _DEFINE_OUTPUT_FIRST(2, 0, name, desc)
#define DEFINE_OUTPUT_8(name, desc) _DEFINE_OUTPUT(2, 1, name, desc)
#define DEFINE_OUTPUT_9(name, desc) _DEFINE_OUTPUT(2, 2, name, desc)

#define DEFINE_OUTPUT_10(name, desc) _DEFINE_OUTPUT_FIRST(3, 0, name, desc)
#define DEFINE_OUTPUT_11(name, desc) _DEFINE_OUTPUT(3, 1, name, desc)
#define DEFINE_OUTPUT_12(name, desc) _DEFINE_OUTPUT(3, 2, name, desc)

#define DEFINE_OUTPUT_13(name, desc) _DEFINE_OUTPUT_FIRST(4, 0, name, desc)
#define DEFINE_OUTPUT_14(name, desc) _DEFINE_OUTPUT(4, 1, name, desc)
#define DEFINE_OUTPUT_15(name, desc) _DEFINE_OUTPUT(4, 2, name, desc)

#define DEFINE_OUTPUT_16(name, desc) _DEFINE_OUTPUT_FIRST(5, 0, name, desc)
#define DEFINE_OUTPUT_17(name, desc) _DEFINE_OUTPUT(5, 1, name, desc)
#define DEFINE_OUTPUT_18(name, desc) _DEFINE_OUTPUT(5, 2, name, desc)

uniform float time;
uniform bool firstPass;

uniform sampler2DRect inputs0;
uniform sampler2DRect inputs1;
uniform sampler2DRect inputs2;
uniform sampler2DRect inputs3;
uniform sampler2DRect inputs4;
uniform sampler2DRect inputs5;
uniform sampler2DRect inputs6;
uniform sampler2DRect inputs7;
uniform sampler2DRect inputs8;
uniform sampler2DRect inputs9;

in vec2 textureCoordinate;

uniform vec2 resolution;

// translate texture coordinates to -1 to 1.
vec2 get_uv_1to1() {
    return (2. * textureCoordinate - resolution.xy) / resolution.y;
}

// translate coordinates in range -1 to 1 to texture coordinates.
vec2 from_uv_1to1(vec2 uv) {
    return ((uv * resolution.y) + resolution.xy) / 2.;
}

float map(float value, float min1, float max1, float min2, float max2) {
    return ((value - min1) / (max1 - min1)) * (max2 - min2) + min2;
}