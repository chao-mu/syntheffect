#ifndef SYNTHEFFECT_RACK_CHANNEL
#define SYNTHEFFECT_RACK_CHANNEL

class ofTexture;

namespace syntheffect {
    namespace rack {
        class Channel {
            public:
                Channel(ofTexture& texture, int idx);
                Channel(float v);

                void copyTo(Channel& channel);

                ofTexture* texture_;
                int idx_;
                float value_;
        };
    }
}

#endif
