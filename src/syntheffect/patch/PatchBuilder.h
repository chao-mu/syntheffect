#pragma once

#include <map>

#include "ofXml.h"

#include "syntheffect/graphics/Effect.h"
#include "syntheffect/graphics/Shader.h"
#include "syntheffect/patch/Patch.h"
#include "syntheffect/patch/Pipeline.h"

namespace syntheffect {
    namespace patch {
        class PatchBuilder {
            public:
                PatchBuilder();
                std::shared_ptr<Patch> build(std::string path, std::shared_ptr<graphics::PingPongBufferMap> buffers);

            private: 
                bool addShader(const ofXml& xml, std::shared_ptr<graphics::PingPongBufferMap> channels, std::shared_ptr<Pipeline> parent);
                bool addShaderParam(const ofXml& xml, std::shared_ptr<graphics::PingPongBufferMap> channels, std::shared_ptr<graphics::Shader> parent);
                bool addPipeline(const ofXml& xml, std::shared_ptr<Patch> patch, std::shared_ptr<graphics::PingPongBufferMap> buffers);
                bool addPipelines(const ofXml& xml, std::shared_ptr<Patch> parent, std::shared_ptr<graphics::PingPongBufferMap> buffers);

                std::map<std::string, std::shared_ptr<graphics::Effect>> effects_by_id_;
        };
    }
}
