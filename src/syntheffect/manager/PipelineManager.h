#pragma once

#include <string>

#include "syntheffect/graphics/Pipeline.h"
#include "syntheffect/graphics/Shader.h"
#include "syntheffect/graphics/PingPongBuffer.h"
#include "syntheffect/graphics/PingPongBufferMap.h"

#include "syntheffect/param/Params.h"

#include "syntheffect/settings/ProjectSettings.h"

namespace syntheffect {
    namespace manager {
        class PipelineManager {
            public:
                void setPipelines(const std::vector<settings::PipelineSettings>& pipelines);

                std::shared_ptr<graphics::Drawable> render(float t, param::Params& params, graphics::PingPongBufferMap& channels);

            private:
                std::map<std::string, graphics::Pipeline> pipelines_;
                std::map<std::string, std::string> pipeline_ins_;

                std::map<std::string, std::vector<std::shared_ptr<graphics::Drawable>>> asset_groups_;
                std::map<std::shared_ptr<graphics::Drawable>, std::string> drawable_names_;
        };
    }
}
