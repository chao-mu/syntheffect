#pragma once

#include <map>

#include "ofXml.h"

#include "syntheffect/settings/ShaderSettings.h"
#include "syntheffect/settings/PipelineSettings.h"
#include "syntheffect/settings/ProjectSettings.h"

#include "syntheffect/param/Param.h"

namespace syntheffect {
    namespace xml {
        class Parser {
            public:
                std::vector<settings::PipelineSettings> parsePipelines(std::string path);
                settings::ProjectSettings parseProject(std::string path);

            private:
                void addPipeline(const ofXml& xml, std::vector<settings::PipelineSettings>& pipelines);
                void addShader(const ofXml& xml, settings::PipelineSettings& pipeline);
                void addShaderParam(const ofXml& xml, settings::ShaderSettings& shader);
        };
    }
}
