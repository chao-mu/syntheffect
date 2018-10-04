#pragma include "../include/header.glsl"

DEFINE_OUTPUT_CHANNEL(0, red, "Red portion of edge detection output")
DEFINE_OUTPUT_CHANNEL(1, green, "Green portion of edge detection output")
DEFINE_OUTPUT_CHANNEL(2, blue, "Blue portion of edge detection output")

uniform bool negate = false;

void mainFrag(inout vec4 col) {
    vec4 n[9] = get3x3();

    vec3 a = applyKernel(KERNEL_SCHARR_X, n).rgb;
    vec3 b = applyKernel(KERNEL_SCHARR_Y, n).rgb;
	vec3 edgeRGB = sqrt((a * a) + (b * b));

    if (negate) {
        edgeRGB = vec3(1) - edgeRGB;
    }

    col.rgb = edgeRGB;
}

#pragma include "../include/footer.glsl"
