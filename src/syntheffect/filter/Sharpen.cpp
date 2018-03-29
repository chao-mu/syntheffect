#include "syntheffect/filter/Sharpen.h"

namespace syntheffect {
    namespace filter {
        Sharpen::Sharpen() : ShaderFilter(std::string("Sharpen"), std::string("Convolution")) {
        }
    }
}