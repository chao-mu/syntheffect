#pragma once

#include "ofTexture.h"

namespace syntheffect {
    namespace param {
        class ParamAccessors {
            public:
                virtual bool getBool(std::string name) = 0;
                virtual int getInt(std::string name) = 0;
                virtual float getFloat(std::string name) = 0;
                virtual ofTexture getTexture(std::string name) = 0;

                virtual bool getBool(std::string name, bool def) = 0;
                virtual int getInt(std::string name, int def) = 0;
                virtual float getFloat(std::string name, float def) = 0;
                virtual ofTexture getTexture(std::string name, ofTexture def) = 0;
        };
    }
}
