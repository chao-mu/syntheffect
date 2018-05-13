#pragma once

#include <map>

#include "syntheffect/graphics/PingPongBuffer.h"

namespace syntheffect {
    namespace graphics {
        class PingPongBufferMap {
            public:
                PingPongBufferMap(int width, int height, int internal_format=GL_RGBA);
                void allocate(std::string key);
                std::shared_ptr<PingPongBuffer> get(std::string key);
                std::map<std::string, bool> getAccessHistory();
                bool isAccessed();

            private:
                std::map<std::string, std::shared_ptr<PingPongBuffer>> buffers_;
                std::map<std::string, bool> access_history_;
                int width_;
                int height_;
                int internal_format_;
        };
    }
}
