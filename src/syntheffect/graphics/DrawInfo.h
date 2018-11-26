#ifndef SYNTHEFFECT_GRAPHICS_DRAWINFO
#define SYNTHEFFECT_GRAPHICS_DRAWINFO

namespace syntheffect {
    namespace graphics {
        class DrawInfo {
            public:
                DrawInfo(float x, float y, float w, float h);
                DrawInfo();
                static DrawInfo scaleCenter(float w, float h, float dest_w, float dest_h);

                float x_, y_, w_, h_;
        };
    }
}
#endif
