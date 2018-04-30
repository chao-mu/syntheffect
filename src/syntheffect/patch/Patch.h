#pragma once

#include "syntheffect/effect/Effect.h"
#include "syntheffect/graphics/PingPongBufferMap.h"
#include "syntheffect/video/Video.h"
#include "syntheffect/patch/Pipeline.h"

namespace syntheffect {
    namespace patch {
        class Patch {
            public:
                Patch();
                void drawTo(shared_ptr<graphics::PingPongBufferMap> channels, float t);
                void addPipeline(shared_ptr<Pipeline> pipeline);

            private:
                vector<shared_ptr<Pipeline>> pipelines_;
        };
    }
}