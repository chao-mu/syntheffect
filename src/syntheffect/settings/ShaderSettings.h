#pragma once

#include <string>
#include <vector>
#include <map>

#include "syntheffect/settings/ParamSettings.h"

namespace syntheffect {
    namespace settings {
        struct ShaderSettings {
            std::string frag;
            std::string vert;
            std::vector<ParamSettings> params;
            std::map<std::string, std::string> texture_params;
        };
    }
}
