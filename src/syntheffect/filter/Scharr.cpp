#include "syntheffect/filter/Scharr.h"

namespace syntheffect {
    namespace filter {
        Scharr::Scharr() : ShaderFilter(std::string("Scharr"), std::string("Convolution")) {
        }
    }
}