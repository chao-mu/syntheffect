#ifndef SYNTHEFFECT_RENDER_RENDERER_H
#define SYNTHEFFECT_RENDER_RENDERER_H

#pragma once

#include <string>

#include "ofTexture.h"

#include "syntheffect/render/Pipeline.h"
#include "syntheffect/graphics/Shader.h"
#include "syntheffect/graphics/PingPongBuffer.h"
#include "syntheffect/graphics/PingPongBufferMap.h"
#include "syntheffect/param/Params.h"
#include "syntheffect/settings/ProjectSettings.h"
#include "syntheffect/asset/Drawable.h"

namespace syntheffect {
    namespace render {
        class Renderer {
            public:
                Renderer(int width, int height);

                void setPipelines(const std::vector<settings::PipelineSettings>& pipelines);
                void setup();

                int getWidth();
                int getHeight();

                void update(param::Params& params, const std::vector<std::shared_ptr<asset::Drawable>>& assets);

                void saveImage(const std::string& path);
                void draw(int width, int height);

            private:
                std::vector<std::shared_ptr<Pipeline>> pipelines_;
                std::map<std::string, std::string> channel_aliases_;
                graphics::PingPongBufferMap channels_;

                std::string lookupName(std::map<std::string, std::string> lookup, std::string name);
                static std::string getLastName(const std::string& name);
        };
    }
}

#endif
