#pragma once

#include <vector>

#include "syntheffect/settings/AssetGroupSettings.h"
#include "syntheffect/settings/ParamSettings.h"
#include "syntheffect/settings/PipelineSettings.h"
#include "syntheffect/settings/JoystickSettings.h"

namespace syntheffect {
    namespace settings {
        struct ProjectSettings {
            std::string pipelines_path;
            std::string assets_path;
            std::string out_path;

            int width;
            int height;

            std::vector<AssetGroupSettings> asset_groups;
            std::vector<PipelineSettings> pipelines;
            std::vector<JoystickSettings> joysticks;
            std::vector<ParamSettings> params;
        };
    }
}
