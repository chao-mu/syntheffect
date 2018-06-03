#include "syntheffect/graphics/Video.h"

namespace syntheffect {
    namespace graphics {
        Video::Video() : ofVideoPlayer(), Drawable() {
        }

        bool Video::load(std::string path) {
            ofVideoPlayer::setUseTexture(true);
            return ofVideoPlayer::load(path);
        }

        void Video::update(std::shared_ptr<graphics::Params> params, float t) {
            ofVideoPlayer::update();
        }

        void Video::draw() {
            ofVideoPlayer::draw(0, 0);
        }

        bool Video::isReady() {
            return isInitialized();
        }
    }
}
