#pragma once

#include <vector>
#include <memory>

#include "syntheffect/graphics/Effect.h"
#include "syntheffect/graphics/PingPongBufferMap.h"
#include "syntheffect/patch/Pipeline.h"

#define CHANNEL_ONE "channel1"
#define CHANNEL_OUT "out"
#define CHANNEL_LAST_OUT "last-out"
#define CHANNEL_LAST_CHANNEL_ONE "last-channel1"

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
