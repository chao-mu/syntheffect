#ifndef SYNTHEFFECT_SETTINGS_PROJECTSETTINGS_H
#define SYNTHEFFECT_SETTINGS_PROJECTSETTINGS_H

#pragma once

#include <vector>

#include "syntheffect/settings/PipelineSettings.h"

namespace syntheffect {
    namespace settings {
        struct ProjectSettings {
            std::string pipelines_path;
            std::string assets_path;
            std::string inputs_path;
            std::string out_path;

            int width;
            int height;

            std::vector<PipelineSettings> pipelines;
        };
    }
}

#endif
