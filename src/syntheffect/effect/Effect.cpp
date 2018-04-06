#include "syntheffect/effect/Effect.h"

namespace syntheffect {
    namespace effect {
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