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
        class Manager {
            public:
                Manager();

                void setGlobalEffectParams(param::Params& params);

                void setProject(const settings::ProjectSettings& project);

                void addDrawable(std::shared_ptr<graphics::Drawable> drawable);
                void setDrawables(std::vector<std::shared_ptr<graphics::Drawable>> drawables);

                int getWidth();
                int getHeight();

                void update(float t);
                bool isReady();

                bool isFinished();

                std::shared_ptr<graphics::Drawable> render();

            private:
                std::string getLastName(std::string buf_name);
                std::string getChannelName(int i);
                std::shared_ptr<syntheffect::graphics::PingPongBufferMap> buildChannels();

                std::map<std::string, graphics::Pipeline> pipelines_;
                std::map<std::string, std::string> pipeline_ins_;

                std::shared_ptr<graphics::PingPongBufferMap> channels_;
                std::vector<std::shared_ptr<graphics::Drawable>> drawables_;
        };
    }
}
