#include "syntheffect/rack/Channel.h"

namespace syntheffect {
    namespace rack {
        Channel::Channel(ofTexture& texture, int idx) : texture_(&texture), idx_(idx) {
        }

        Channel::Channel(float v) : texture_(nullptr), idx_(-1), value_(v) {
        }

        void Channel::copyTo(Channel& channel) {
            channel.texture_ = texture_;
            channel.value_ = value_;
            channel.idx_ = idx_;
        }
    }
}
