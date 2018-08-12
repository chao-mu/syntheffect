#pragma once

#include <map>

#include "ofXml.h"

#include "syntheffect/settings/ShaderSettings.h"
#include "syntheffect/settings/ParamSettings.h"
#include "syntheffect/settings/PipelineSettings.h"
#include "syntheffect/settings/AssetGroupSettings.h"
#include "syntheffect/settings/ProjectSettings.h"
#include "syntheffect/settings/JoystickSettings.h"

namespace syntheffect {
    namespace xml {
        class Parser {
            public:
                std::vector<settings::PipelineSettings> parsePipelines(std::string path);
                std::vector<settings::AssetGroupSettings> parseAssets(std::string path);
                settings::ProjectSettings parseProject(std::string path);

            private:
                settings::ParamSettings parseParam(const ofXml& xml, bool require_name=true);
                void addJoystick(const ofXml& xml, std::string path, std::vector<settings::JoystickSettings>& joysticks);
                void addAssetGroup(const ofXml& xml, std::string path, std::vector<settings::AssetGroupSettings>& asset_groups);
                void addAsset(const ofXml& xml, std::string path, settings::AssetGroupSettings& asset_group);
                void addTriggerParam(const ofXml& xml, settings::AssetGroupSettings& asset_group);
                void addPipeline(const ofXml& xml, std::vector<settings::PipelineSettings>& pipelines);
                void addShader(const ofXml& xml, settings::PipelineSettings& pipeline);
                void addShaderParam(const ofXml& xml, settings::ShaderSettings& shader);
        };
    }
}
