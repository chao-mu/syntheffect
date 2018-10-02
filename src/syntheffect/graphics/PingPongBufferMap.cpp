#include "syntheffect/graphics/PingPongBufferMap.h"

#include "ofLog.h"

namespace syntheffect {
    namespace graphics {
        PingPongBufferMap::PingPongBufferMap(int internal_format) : buffers_() {
            internal_format_ = internal_format;
        }

        std::vector<std::string> PingPongBufferMap::getKeys() {
            std::vector<std::string> keys;
            for (const auto& kv : buffers_) {
                keys.push_back(kv.first);
            }

            return keys;
        }

        bool PingPongBufferMap::exists(std::string key) {
            return buffers_.count(key) > 0;
        }

        std::shared_ptr<PingPongBuffer> PingPongBufferMap::get_or_allocate(std::string key, int width, int height) {
            if (exists(key)) {
                return at(key);
            }

            std::shared_ptr<PingPongBuffer> buf = std::make_shared<PingPongBuffer>();
            buf->allocate(width, height, internal_format_);
            buffers_[key] = buf;

            return buf;
        }


        std::shared_ptr<PingPongBuffer> PingPongBufferMap::at(std::string key) {
            if (buffers_.count(key) == 0) {
                throw std::out_of_range("Accessing non-existent buffer with key '" + key + "'");
            }

            return buffers_.at(key);
        }
    }
}
