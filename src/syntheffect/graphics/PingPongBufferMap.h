#ifndef SYNTHEFFECT_GRAPHICS_PINGPONGBUFFERMAP_H
#define SYNTHEFFECT_GRAPHICS_PINGPONGBUFFERMAP_H

#pragma once

#include <map>

#include "syntheffect/graphics/PingPongBuffer.h"

namespace syntheffect {
    namespace graphics {
        class PingPongBufferMap {
            public:
                PingPongBufferMap(int internal_format=GL_RGBA);
                std::shared_ptr<PingPongBuffer> get_or_allocate(std::string key, int width, int height);
                std::shared_ptr<PingPongBuffer> at(std::string key);
                bool exists(std::string key);

                std::vector<std::string> getKeys();
                int getHeight();
                int getWidth();

            private:
                std::map<std::string, std::shared_ptr<PingPongBuffer>> buffers_;
                int internal_format_;
        };
    }
}

#endif
