#ifndef SYNTHEFFECT_ASSET_ASSET_H
#define SYNTHEFFECT_ASSET_ASSET_H

#pragma once

#include <memory>

#include "syntheffect/graphics/PingPongBuffer.h"

namespace syntheffect {
    namespace asset {
        class Asset {
            public:
                Asset(std::string id);

                virtual void setup();
                virtual void update(float t);
                virtual bool isReady();
                virtual bool isFinished();

                virtual void restart();
                virtual void play();
                virtual void pause();

                std::string getID();
                void setID(std::string id);

                bool isActive();
                void setActive(bool active);

                std::string getStack();
                void setStack(std::string stack);

                std::string getGroup();
                void setGroup(std::string stack);

            private:
                std::string id_;
                std::string stack_;
                std::string group_;
                bool active_;
        };
    }
}

#endif
