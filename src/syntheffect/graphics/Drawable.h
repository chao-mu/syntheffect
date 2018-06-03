#pragma once

#include <memory>

#include "syntheffect/graphics/Params.h"

namespace syntheffect {
    namespace graphics {
        class Drawable {
            public:
                virtual void update(std::shared_ptr<Params> params, float t) = 0;
                virtual void draw() = 0;
                virtual bool isReady() { return true; };
        };
    }
}
