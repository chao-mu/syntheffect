#ifndef SYNTHEFFECT_RACK_GLOBAL
#define SYNTHEFFECT_RACK_GLOBAL

#include "syntheffect/rack/Module.h"

namespace syntheffect {
    namespace rack {
        class Global : public Module {
            public:
                Global(const std::string& id, int width, int height);

                virtual void update(float t) override;
                virtual void setup(int width, int height, int internal_format) override;
                virtual const std::string getType() override;

                static const std::string getModuleType();


                const int width_;
                const int height_;
        };
    }
}
#endif
