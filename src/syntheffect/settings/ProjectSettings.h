#pragma once

#include <vector>

#include "syntheffect/settings/AssetSettings.h"
#include "syntheffect/settings/PipelineSettings.h"

namespace syntheffect {
    namespace settings {
        struct ProjectSettings {
            //std::vector<AssetSettings> assets;
            //std::vector<InputSettings> inputs;
            std::vector<PipelineSettings> pipelines;
        };
    }
}
