#pragma once

#include "ofXml.h"

#include "syntheffect/effect/Effect.h"
#include "syntheffect/effect/Shader.h"
#include "syntheffect/patch/Patch.h"
#include "syntheffect/patch/Pipeline.h"

namespace syntheffect {
    namespace patch {
        class PatchBuilder {
            public:
                PatchBuilder();
                shared_ptr<Patch> build(std::string path, shared_ptr<graphics::PingPongBufferMap> buffers);

            private: 
                bool addShader(ofXml& xml, shared_ptr<Pipeline> parent);
                bool addShaderParam(ofXml& xml, shared_ptr<effect::Shader> parent);
                bool addPipeline(ofXml& xml, shared_ptr<Patch> patch, shared_ptr<graphics::PingPongBufferMap> buffers);
                bool addPipelines(ofXml& xml, shared_ptr<Patch> parent, shared_ptr<graphics::PingPongBufferMap> buffers);
                bool addControls(ofXml& xml, shared_ptr<Patch> parent);

                map<std::string, shared_ptr<effect::Effect>> effects_by_id_;
        };
    }
}