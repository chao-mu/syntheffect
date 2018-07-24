#pragma once

#include <vector>
#include <memory>

#include "syntheffect/graphics/Effect.h"
#include "syntheffect/graphics/PingPongBufferMap.h"
#include "syntheffect/patch/Pipeline.h"

#define CHANNEL_OUT "out"
#define CHANNEL_LAST_OUT "last-out"

namespace syntheffect {
    namespace patch {
        class Patch {
            public:
                Patch();
                void drawTo(std::shared_ptr<graphics::PingPongBufferMap> channels);
                void addPipeline(std::shared_ptr<Pipeline> pipeline);
                void setEffectParams(std::shared_ptr<param::Params> params);

            private:
                std::vector<std::shared_ptr<Pipeline>> pipelines_;
        };
    }
}
