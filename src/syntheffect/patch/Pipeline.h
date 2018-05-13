#pragma once

#include "syntheffect/graphics/Effect.h"
#include "syntheffect/graphics/PingPongBufferMap.h"
#include "syntheffect/video/Video.h"

namespace syntheffect {
    namespace patch {
        class Pipeline {
            public:
                Pipeline(std::string in, std::string out);

                void drawTo(std::shared_ptr<graphics::PingPongBufferMap> channels, float t);

                void addEffect(std::shared_ptr<graphics::Effect> pipeline_effect);

                std::string getIn();
                std::string getOut();
                std::vector<std::shared_ptr<graphics::Effect>> getEffects();

            private:
                std::vector<std::shared_ptr<graphics::Effect>> effects_;
                std::string in_;
                std::string out_;
        };
    }
}
