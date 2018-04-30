#include "syntheffect/graphics/PingPongBufferMap.h"

#include "ofLog.h"

namespace syntheffect {
    namespace graphics {
        PingPongBufferMap::PingPongBufferMap(int width, int height, int internal_format) : buffers_() {
            width_ = width;
            height_ = height;
            internal_format_ = internal_format;
        }

        void PingPongBufferMap::allocate(std::string key) {
            if (buffers_.count(key) > 0) {
                return;
            }

            shared_ptr<PingPongBuffer> buf = make_shared<PingPongBuffer>();
            buf->allocate(width_, height_, internal_format_);
            buffers_[key] = buf;
            access_history_[key] = false;
        }

        shared_ptr<PingPongBuffer> PingPongBufferMap::get(std::string key) {
            access_history_[key] = true;
            if (buffers_.count(key) == 0) {
                ofLogError() << "Accessing non-existent buffer with key '" << key << "'";
            }

            return buffers_.at(key);
        }

        std::map<std::string, bool> PingPongBufferMap::getAccessHistory() {
            return access_history_;
        }
    }
}
