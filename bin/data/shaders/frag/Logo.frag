#pragma include "include/module.glsl"

// Produce a 1 if the provided value is <= 0, with a small transitional blur
#define S(v) smoothstep(blur, 0., v)

// Draw (return 1) a circle at the origin with the radius of r.
#define circle(U, r) S(length(U) - r)

// Draw (return 1) a crescent made from two overlapping circles.
// The overlap is controlled by the provided offset. The two radiuses are determined by r.
#define crescent(U, offset, r) circle(U, r) - circle( U - vec2(offset,0), r)

// Draw a line segment of length and width.
#define segment(U, len, width)  S( abs(U.x)-width ) * S( abs(U.y)-len )

DEFINE_OUTPUT_1(value, DESC("the logo"))

void main()
{
    // Normalized pixel coordinates (from -1 to 1), so that we can
    // draw around the origin and be at the center.
    vec2 uv = get_uv_1to1();

    // Define a gradient to use at edges that is small relative to the size of the screen
    // this allows to work on multiple resolutions
    // This value is used in our macros.
    float blur = 2. / resolution.y;

    // Draw on either side of the y axis.
    uv.x = abs(uv.x);

    float value = 0.;
    value += crescent(uv - vec2(.37, 0.1),  .18, .4);
    value += segment(uv, .75, .075);
    value += segment((uv + vec2(0, 0.47)), .075, .35);

    output_value(clamp(value, 0., 1.));
}
