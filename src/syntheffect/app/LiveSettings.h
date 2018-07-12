#pragma once

#include <string>
#include <vector>
#include <memory>

#include "syntheffect/graphics/Drawable.h"

namespace syntheffect {
    namespace app {
        class LiveSettings {
            public:
                std::string patch_path;
                std::string out_path;

                int recording_width;
                int recording_height;

                std::vector<std::shared_ptr<graphics::Drawable>> drawables;
        };
    }
}
