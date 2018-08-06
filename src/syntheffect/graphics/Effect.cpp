#include "syntheffect/graphics/Effect.h"

#include "ofTexture.h"

namespace syntheffect {
    namespace graphics {
        Effect::Effect() {
            active_ = true;
        }

        bool Effect::isActive() {
            return active_;
        }

        void Effect::toggle() {
            if (isActive()) {
                stop();
            } else {
                start();
            }
        }

        void Effect::start() {
            active_ = true;
        }

        void Effect::stop() {
            active_ = false;
        }
    }
}
