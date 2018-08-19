#pragma once

#include "ofXml.h"

#include "syntheffect/settings/ParamSettings.h"

namespace syntheffect {
    namespace param {
        class Parser {
            public:
                static settings::ParamSettings parseParam(const ofXml& xml, bool require_default=false);
        };
    }
}
