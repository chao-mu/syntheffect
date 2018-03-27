#include "syntheffect/filter/Sobel.h"

namespace syntheffect {
    namespace filter {
        Sobel::Sobel() : ShaderFilter(std::string("Sobel"), std::string("Convolution")) {
        }
    }
}