#pragma once

#include "ofXml.h"

namespace syntheffect {
    namespace xml {
        class Util {
            public:
                template<typename T>
                static T getAttribute(const ofXml& xml, std::string name, bool required, T def);
        };
    }
}

#include "syntheffect/xml/Util.tpl"

