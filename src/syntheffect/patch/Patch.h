#pragma once

#include "syntheffect/graphics/Effect.h"
#include "syntheffect/graphics/PingPongBufferMap.h"
#include "syntheffect/video/Video.h"
#include "syntheffect/patch/Pipeline.h"

namespace syntheffect {
    namespace patch {
        class Patch {
            public:
                Patch();
                void drawTo(std::shared_ptr<graphics::PingPongBufferMap> channels, float t);
                void addPipeline(std::shared_ptr<Pipeline> pipeline);
                void setEffectParams(std::shared_ptr<graphics::Params> params);

            private:
                std::vector<std::shared_ptr<Pipeline>> pipelines_;
        };
    }
}
