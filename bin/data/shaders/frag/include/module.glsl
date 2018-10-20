#version 330

#define PI 3.14159265359
#define PI_HALF 1.57079632679

#define DESC(s) #s

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
    }

#define DEFINE_OUTPUT_X(tex_idx, channel_idx, name, desc) \
    void output_ ## name ## (float x) { \
        output ## tex_idx ## [channel_idx] = x; \
        output ## tex_idx ## [3] = 1.0; \
    }

#define DEFINE_OUTPUT_1(name, desc) \
    layout (location = 0) out vec4 output0; \
    DEFINE_OUTPUT_X(0, 0, name, desc)
#define DEFINE_OUTPUT_2(name, desc) DEFINE_OUTPUT_X(0, 1, name, desc)
#define DEFINE_OUTPUT_3(name, desc) DEFINE_OUTPUT_X(0, 2, name, desc)

#define DEFINE_OUTPUT_4(name, desc) \
    layout (location = 1) out vec4 output1; \
    DEFINE_OUTPUT_X(1, 0, name, desc)
#define DEFINE_OUTPUT_5(name, desc) DEFINE_OUTPUT_X(1, 1, name, desc)
#define DEFINE_OUTPUT_6(name, desc) DEFINE_OUTPUT_X(1, 2, name, desc)

#define DEFINE_OUTPUT_7(name, desc) \
    layout (location = 2) out vec4 output2; \
    DEFINE_OUTPUT_X(2, 0, name, desc)
#define DEFINE_OUTPUT_8(name, desc) DEFINE_OUTPUT_X(2, 1, name, desc)
#define DEFINE_OUTPUT_9(name, desc) DEFINE_OUTPUT_X(2, 2, name, desc)

#define DEFINE_OUTPUT_10(name, desc) \
    layout (location = 3) out vec4 output3; \
    DEFINE_OUTPUT_X(3, 0, name, desc)
#define DEFINE_OUTPUT_11(name, desc) DEFINE_OUTPUT_X(3, 1, name, desc)
#define DEFINE_OUTPUT_12(name, desc) DEFINE_OUTPUT_X(3, 2, name, desc)

#define DEFINE_OUTPUT_13(name, desc) \
    layout (location = 4) out vec4 output4; \
    DEFINE_OUTPUT_X(4, 0, name, desc)
#define DEFINE_OUTPUT_14(name, desc) DEFINE_OUTPUT_X(4, 1, name, desc)
#define DEFINE_OUTPUT_15(name, desc) DEFINE_OUTPUT_X(4, 2, name, desc)

#define DEFINE_OUTPUT_16(name, desc) \
    layout (location = 5) out vec4 output5; \
    DEFINE_OUTPUT_X(5, 0, name, desc)
#define DEFINE_OUTPUT_17(name, desc) DEFINE_OUTPUT_X(5, 1, name, desc)
#define DEFINE_OUTPUT_18(name, desc) DEFINE_OUTPUT_X(5, 2, name, desc)

uniform float time;

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
