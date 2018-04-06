#pragma once

#include "ofXml.h"

#include "syntheffect/effect/Effect.h"
#include "syntheffect/effect/Shader.h"
#include "syntheffect/patch/Patch.h"

namespace syntheffect {
    namespace patch {
        class PatchBuilder {
            public:
                shared_ptr<Patch> build(std::string path);

            private: 
                bool addShader(ofXml& xml, shared_ptr<Patch> p);
                bool addShaderParam(ofXml& xml, shared_ptr<effect::Shader> shader);
                bool addPipeline(ofXml& xml, shared_ptr<Patch> p);
                bool addControls(ofXml& xml, shared_ptr<Patch> p);
        };
    }
}