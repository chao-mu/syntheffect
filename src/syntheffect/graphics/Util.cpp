#include "syntheffect/graphics/Util.h"

namespace syntheffect {
    namespace graphics {
        void Util::drawScaleCenter(float w, float h, float dest_width, float dest_height, std::function<void(float, float, float, float)> draw) {
            float aspect = w / h;
            float dest_aspect = dest_width / dest_height;

            float scale_factor;
            if (dest_aspect > aspect) {
                scale_factor = dest_height / h;
            } else {
                scale_factor = dest_width / w;
            }

            float draw_height = h * scale_factor;
            float draw_width = w * scale_factor;

            draw((dest_width / 2.0) - (draw_width / 2.0),
                (dest_height / 2.0) - (draw_height / 2.0),
                draw_width, draw_height);
        }
    }
}
