#pragma once

#include "ofXml.h"

#include "syntheffect/param/Param.h"

namespace syntheffect {
    namespace param {
        class Parser {
            public:
                static Param parseParam(const ofXml& xml, bool require_default=false);
        };
    }
}
