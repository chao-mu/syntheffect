#include "syntheffect/asset/Asset.h"

namespace syntheffect {
    namespace asset {
        Asset::Asset(std::string name) {
            name_ = name;
        }

        std::string Asset::getName() {
            return name_;
        }

        bool Asset::isReady() {
            return true;
        }

        bool Asset::isFinished() {
            return false;
        }

        void Asset::setup() {
        }

        void Asset::update(float) {
        }

        void Asset::drawScaleCenter(float dest_width, float dest_height) {
            float w = getWidth();
            float h = getHeight();

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
