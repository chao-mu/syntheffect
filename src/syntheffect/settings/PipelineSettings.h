#pragma once

#include <string>
#include <vector>

#include "syntheffect/settings/ShaderSettings.h"

namespace syntheffect {
    namespace settings {
        struct PipelineSettings {
            std::string in;
            std::string out;
            std::vector<ShaderSettings> shaders;
        };
    }
}
