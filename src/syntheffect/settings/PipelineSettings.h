#ifndef SYNTHEFFECT_SETTINGS_PIPELINESETTINGS_H
#define SYNTHEFFECT_SETTINGS_PIPELINESETTINGS_H

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

#endif
