#pragma once

#include <string>

#include "ofTexture.h"

#include "syntheffect/render/Pipeline.h"
#include "syntheffect/graphics/Shader.h"
#include "syntheffect/graphics/PingPongBuffer.h"
#include "syntheffect/graphics/PingPongBufferMap.h"
#include "syntheffect/param/Params.h"
#include "syntheffect/settings/ProjectSettings.h"
#include "syntheffect/asset/Asset.h"

namespace syntheffect {
    namespace render {
        class Renderer {
            public:
                Renderer(int width, int height);

                void setPipelines(const std::vector<settings::PipelineSettings>& pipelines);
                void setup();

                int getWidth();
                int getHeight();

                void update(param::Params& params, const std::vector<std::shared_ptr<asset::Asset>>& assets);

                void saveImage(std::string path);
                void draw(int width, int height);

            private:
                std::vector<std::shared_ptr<Pipeline>> pipelines_;
                graphics::PingPongBufferMap channels_;

                std::string getLastName(std::string name);
        };
    }
}
