#include "syntheffect/filter/FilterBase.h"
#include "syntheffect/filter/HasIntensityBase.h"

namespace syntheffect {
    namespace filter {
        class Delay : public FilterBase, public HasIntensityBase {
            public:
                Delay(bool active=false);
                void setLastTexture(ofTexture last);

            protected:    
                void setupUniforms();
                ofTexture last_texture_;
        };
    }
}