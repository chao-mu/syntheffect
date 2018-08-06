#pragma once

#include <string>

#include "syntheffect/graphics/Pipeline.h"
#include "syntheffect/graphics/Shader.h"
#include "syntheffect/graphics/PingPongBuffer.h"
#include "syntheffect/graphics/PingPongBufferMap.h"

#include "syntheffect/param/Params.h"

namespace syntheffect {
    namespace manager {
        class Manager {
            public:
                Manager();

                int appendShaderEffect(int pipeline_id, std::string frag, std::string vert);
                void setEffectParams(int pipeline_id, int effect_index, param::Params& params);
                void setGlobalEffectParams(param::Params& params);

                int addPipeline();
                void connectPipelineIn(int pipeline_id, std::string buffer_in);
                void connectPipelineOut(int pipeline_id, std::string buffer_out);

                void addDrawable(std::shared_ptr<graphics::Drawable> drawable);
                void setDrawables(std::vector<std::shared_ptr<graphics::Drawable>> drawables);

                int getWidth();
                int getHeight();

                void update(float t);
                bool isReady();

                bool isFinished();

                std::shared_ptr<graphics::Drawable> render();

            private:
                int nextID();
                std::string getLastName(std::string buf_name);
                std::string getChannelName(int i);
                std::shared_ptr<syntheffect::graphics::PingPongBufferMap> buildChannels();

                int current_id_;

                std::map<int, graphics::Pipeline> pipelines_;
                std::map<int, std::string> pipeline_ins_;
                std::map<int, std::string> pipeline_outs_;
                std::shared_ptr<graphics::PingPongBufferMap> channels_;
                std::vector<std::shared_ptr<graphics::Drawable>> drawables_;
        };
    }
}
