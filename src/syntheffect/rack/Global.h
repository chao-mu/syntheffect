#ifndef SYNTHEFFECT_RACK_GLOBAL
#define SYNTHEFFECT_RACK_GLOBAL

#include "syntheffect/rack/Module.h"

namespace syntheffect {
    namespace rack {
        class Global : public Module {
            public:
                Global(const std::string& id);

                virtual void update(float t) override;
                virtual void setup(int width, int height, int internal_format) override;
        };
    }
}
#endif
