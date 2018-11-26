#ifndef SYNTHEFFECT_RACK_WEBCAM
#define SYNTHEFFECT_RACK_WEBCAM

#include <string>

#include "ofVideoGrabber.h"
#include "ofFbo.h"

#include "syntheffect/rack/Module.h"
#include "syntheffect/graphics/DrawInfo.h"

namespace syntheffect {
    namespace rack {
        class Webcam : public Module {
            public:
                Webcam(const std::string& id);

                virtual void setup(int width, int height, int internal_format, const std::string& workspace_dir) override;
                virtual void update(float t) override;
                virtual bool isReady() override;
                virtual const std::string getType() override;

                static const std::string getModuleType();

            private:
                const std::string path_;
                ofVideoGrabber player_;
                ofFbo outputs_;
                graphics::DrawInfo draw_info_;
        };
    }
}

#endif
