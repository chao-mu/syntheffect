#include "syntheffect/graphics/PingPongBufferMap.h"

#include "ofLog.h"

namespace syntheffect {
    namespace graphics {
        PingPongBufferMap::PingPongBufferMap(int width, int height, int internal_format) : buffers_() {
            width_ = width;
            height_ = height;
            internal_format_ = internal_format;
        }

        int PingPongBufferMap::getHeight() {
            return height_;
        }

        int PingPongBufferMap::getWidth() {
            return width_;
        }

        std::vector<std::string> PingPongBufferMap::getKeys() {
            std::vector<std::string> keys;
            for (const auto& kv : buffers_) {
                keys.push_back(kv.first);
            }

            return keys;
        }

        void PingPongBufferMap::allocate(std::string key) {
            if (buffers_.count(key) > 0) {
                return;
            }

            std::shared_ptr<PingPongBuffer> buf = std::make_shared<PingPongBuffer>();
            buf->allocate(width_, height_, internal_format_);
            buffers_[key] = buf;
        }

        std::shared_ptr<PingPongBuffer> PingPongBufferMap::get_or_allocate(std::string key) {
            if (buffers_.count(key) == 0) {
                allocate(key);
            }

            return buffers_.at(key);
        }


        std::shared_ptr<PingPongBuffer> PingPongBufferMap::get(std::string key) {
            if (buffers_.count(key) == 0) {
                throw std::runtime_error("Accessing non-existent buffer with key '" + key + "'");
            }

            return buffers_.at(key);
        }
    }
}
