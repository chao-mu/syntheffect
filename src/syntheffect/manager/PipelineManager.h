#pragma once

#include <string>

#include "syntheffect/graphics/Pipeline.h"
#include "syntheffect/graphics/Shader.h"
#include "syntheffect/graphics/PingPongBuffer.h"
#include "syntheffect/graphics/PingPongBufferMap.h"

#include "syntheffect/param/Params.h"

#include "syntheffect/settings/ProjectSettings.h"
#include "syntheffect/settings/AssetGroupSettings.h"

namespace syntheffect {
    namespace manager {
        class PipelineManager {
            public:
                PipelineManager();

                void setGlobalParams(param::Params& params);

                void setPipelines(const std::vector<settings::PipelineSettings>& pipelines);
                void setAssets(const std::vector<settings::AssetGroupSettings>& asset_groups);

                int getWidth();
                int getHeight();

                void update(float t);
                bool isReady();

                bool isFinished();

                std::shared_ptr<graphics::Drawable> render();

            private:
                param::Params global_params_;
                std::string active_asset_group_;

                std::map<std::string, std::shared_ptr<graphics::Drawable>> getDrawables();
                std::string getLastName(std::string buf_name);
                std::string getChannelName(int i);
                std::shared_ptr<syntheffect::graphics::PingPongBufferMap> buildChannels();
                std::string getTriggerName(std::string group_name);

                std::map<std::string, graphics::Pipeline> pipelines_;
                std::map<std::string, std::string> pipeline_ins_;

                std::shared_ptr<graphics::PingPongBufferMap> channels_;

                std::map<std::string, std::vector<std::shared_ptr<graphics::Drawable>>> asset_groups_;
                std::map<std::shared_ptr<graphics::Drawable>, std::string> drawable_names_;
        };
    }
}
