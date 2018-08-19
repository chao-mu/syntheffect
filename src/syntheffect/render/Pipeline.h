#pragma once

#include "syntheffect/graphics/Effect.h"
#include "syntheffect/graphics/PingPongBufferMap.h"

namespace syntheffect {
    namespace render {
        class Pipeline {
            public:
                Pipeline(std::string in, std::string out);
                void drawTo(std::shared_ptr<graphics::PingPongBuffer> buf_in, std::shared_ptr<graphics::PingPongBuffer> buf_out);

                int addEffect(std::shared_ptr<graphics::Effect> pipeline_effect);

                std::vector<std::shared_ptr<graphics::Effect>> getEffects();

                std::string getOut();
                void setOut(std::string out);

                std::string getIn();
                void setIn(std::string in);

            private:
                std::vector<std::shared_ptr<graphics::Effect>> effects_;
                int nextID();
                int current_id_;
                std::string in_;
                std::string out_;
        };
    }
}
