#ifndef SYNTHEFFECT_RACK_WEBCAM
#define SYNTHEFFECT_RACK_WEBCAM

#include <string>

#include "ofVideoGrabber.h"
#include "ofFbo.h"

#include "syntheffect/rack/Module.h"

namespace syntheffect {
    namespace rack {
        class Webcam : public Module {
            public:
                Webcam(const std::string& id);

                virtual void setup(int width, int height, int internal_format) override;
                virtual void update(float t) override;
                virtual bool isReady() override;

            private:
                const std::string path_;
                ofVideoGrabber player_;
                ofFbo outputs_;
        };
    }
}

#endif
