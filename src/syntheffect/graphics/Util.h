#ifndef SYNTHEFFECT_GRAPHICS_UTIL
#define SYNTHEFFECT_GRAPHICS_UTIL

#include <functional>

namespace syntheffect {
    namespace graphics {
        class Util {
            public:
                static void drawScaleCenter(float w, float h, float dest_w, float dest_h, std::function<void(float, float, float, float)> draw);
        };
    }
}
#endif
