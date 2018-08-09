#pragma once

#include <map>

#include "ofXml.h"

#include "syntheffect/manager/Manager.h"

#include "syntheffect/settings/ShaderSettings.h"
#include "syntheffect/settings/ParamSettings.h"
#include "syntheffect/settings/PipelineSettings.h"
#include "syntheffect/settings/ProjectSettings.h"

namespace syntheffect {
    namespace xml {
        class Parser {
            public:
                void parse(std::string path, std::shared_ptr<manager::Manager> m);

            private:
                void addPipelines(const ofXml& xml, std::shared_ptr<manager::Manager> m);
                void addPipeline(const ofXml& xml, settings::ProjectSettings& project);
                void addShader(const ofXml& xml, settings::PipelineSettings& pipeline);
                void addShaderParam(const ofXml& xml, settings::ShaderSettings& shader);
        };
    }
}
