#include "syntheffect/graphics/DrawInfo.h"

namespace syntheffect {
    namespace graphics {
        DrawInfo::DrawInfo(float x, float y, float w, float h) : x_(x), y_(y), w_(w), h_(h) {}
        DrawInfo::DrawInfo() : x_(0), y_(0), w_(0), h_(0) {}

        DrawInfo DrawInfo::scaleCenter(float w, float h, float dest_w, float dest_h) {
            float aspect = w / h;
            float dest_aspect = dest_w / dest_h;

            float draw_height, draw_width;
            if (dest_aspect > aspect) {
                draw_width = w * dest_h / h;
                draw_height = dest_h;
            } else {
                draw_width = dest_w;
                draw_height = h * dest_w / w;
            }

            return DrawInfo(
                (dest_w - draw_width) / 2.0,
                (dest_h - draw_height) / 2.0,
                draw_width,
                draw_height
            );
        }
    }
}
