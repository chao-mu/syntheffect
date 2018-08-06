#pragma once

#include "syntheffect/graphics/Effect.h"
#include "syntheffect/graphics/PingPongBufferMap.h"

namespace syntheffect {
    namespace graphics {
        class Pipeline {
            public:
                void drawTo(std::shared_ptr<graphics::PingPongBuffer> buf_in, std::shared_ptr<graphics::PingPongBuffer> buf_out);

                int addEffect(std::shared_ptr<graphics::Effect> pipeline_effect);

                std::vector<std::shared_ptr<graphics::Effect>> getEffects();

            private:
                std::vector<std::shared_ptr<graphics::Effect>> effects_;
                int nextID();
                int current_id_;
        };
    }
}
