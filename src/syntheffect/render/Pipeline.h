#ifndef SYNTHEFFECT_RENDER_PIPELINE_H
#define SYNTHEFFECT_RENDER_PIPELINE_H

#pragma once

#include "syntheffect/graphics/Effect.h"
#include "syntheffect/graphics/PingPongBufferMap.h"

namespace syntheffect {
    namespace render {
        class Pipeline {
            public:
                Pipeline(const std::string& in, const std::string& out);
                void drawTo(std::shared_ptr<graphics::PingPongBuffer> buf_in, std::shared_ptr<graphics::PingPongBuffer> buf_out);

                int addEffect(std::shared_ptr<graphics::Effect> pipeline_effect);

                std::vector<std::shared_ptr<graphics::Effect>> getEffects();

                std::string getOut();
                void setOut(const std::string& out);

                std::string getIn();
                void setIn(const std::string& in);

            private:
                std::vector<std::shared_ptr<graphics::Effect>> effects_;
                int nextID();
                int current_id_;
                std::string in_;
                std::string out_;
        };
    }
}

#endif
