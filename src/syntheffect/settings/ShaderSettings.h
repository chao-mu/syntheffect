#pragma once

#include <string>
#include <vector>
#include <map>

#include "syntheffect/param/Param.h"

namespace syntheffect {
    namespace settings {
        struct ShaderSettings {
            std::string frag;
            std::string vert;
            std::vector<param::Param> params;
            std::map<std::string, std::string> texture_params;
        };
    }
}
