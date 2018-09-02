#ifndef SYNTHEFFECT_GRAPHICS_PINGPONGBUFFERMAP_H
#define SYNTHEFFECT_GRAPHICS_PINGPONGBUFFERMAP_H

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

                std::vector<std::string> getKeys();
                int getHeight();
                int getWidth();

            private:
                std::map<std::string, std::shared_ptr<PingPongBuffer>> buffers_;
                int width_;
                int height_;
                int internal_format_;
        };
    }
}

#endif
