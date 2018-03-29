#include "syntheffect/filter/Blur.h"

namespace syntheffect {
    namespace filter {
        Blur::Blur() : ShaderFilter(std::string("Blur"), std::string("Convolution")) {
        }
    }
}