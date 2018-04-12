#pragma once

#include "syntheffect/effect/Effect.h"
#include "syntheffect/graphics/PingPongBufferMap.h"
#include "syntheffect/video/Video.h"

namespace syntheffect {
    namespace patch {
        class Pipeline {
            public:
                Pipeline(std::string in, std::string out);

                void draw(shared_ptr<graphics::PingPongBufferMap> channels, float t);

                void addEffect(shared_ptr<effect::Effect> pipeline_effect);

                std::string getIn();
                std::string getOut();
                std::string in_;
                std::string out_;

            private:
                std::vector<shared_ptr<effect::Effect>> effects_;
        };
    }
}