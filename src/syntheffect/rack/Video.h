#ifndef SYNTHEFFECT_RACK_VIDEO
#define SYNTHEFFECT_RACK_VIDEO

#include <string>

#include "ofVideoPlayer.h"
#include "ofFbo.h"

#include "syntheffect/rack/Module.h"

namespace syntheffect {
    namespace rack {
        class Video : public Module {
            public:
                Video(const std::string& id, const std::string& path);

                virtual void setup(int width, int height, int internal_format) override;
                virtual void start() override;
                virtual bool isReady() override;
                virtual void update(float t) override;
                virtual const std::string getType() override;

                static const std::string getModuleType();

            private:
                const std::string path_;
                ofVideoPlayer player_;
                ofFbo outputs_;
        };
    }
}

#endif
