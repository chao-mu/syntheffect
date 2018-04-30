#pragma once

#include "ofXml.h"
#include "ofTexture.h"

#include "syntheffect/graphics/PingPongBuffer.h"

namespace syntheffect {
    namespace effect {
        class Effect {
            public:
                Effect();

                virtual void drawTo(shared_ptr<graphics::PingPongBuffer> ping_ping, float t) = 0;
                void setParam(std::string param, std::function<float()> get);
                void setParam(std::string param, std::function<bool()> get);
                void setParam(std::string param, std::function<int()> get);
                void setParam(std::string param, std::function<ofTexture()> get);
                void toggle();
                void start();
                void stop();
                bool isActive();

            protected:
                std::map<std::string, std::function<float()>> float_params_;
                std::map<std::string, std::function<int()>> int_params_;
                std::map<std::string, std::function<bool()>> bool_params_;
                std::map<std::string, std::function<ofTexture()>> texture_params_;

            private:
                bool active_;
        };
    }
}