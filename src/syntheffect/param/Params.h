#pragma once

#include <map>
#include <variant>

#include "ofTexture.h"

#include "syntheffect/param/ParamAccessors.h"

#include "syntheffect/settings/ParamSettings.h"

namespace syntheffect {
    namespace param {
        class Params {
            public:
                void set(settings::ParamSettings p);
                void setTexture(std::string name, std::function<ofTexture()> v);
                void setTexture(std::string name, std::string target);

                float resolveValue(const settings::ParamSettings& p);
                settings::ParamSettings resolveVariable(const settings::ParamSettings& p);

                bool getBool(std::string name);
                int getInt(std::string name);
                float getFloat(std::string name);
                ofTexture getTexture(std::string name);

                bool getBool(std::string name, bool def);
                int getInt(std::string name, int def);
                float getFloat(std::string name, float def);
                ofTexture getTexture(std::string name, ofTexture def);

                std::map<std::string, bool> getBools();
                std::map<std::string, int> getInts();
                std::map<std::string, float> getFloats();
                std::map<std::string, ofTexture> getTextures();

                void copyTo(Params& p) const;

                bool exists(std::string name);

                bool isInt(const settings::ParamSettings& p);
                bool isBool(const settings::ParamSettings& p);
                bool isFloat(const settings::ParamSettings& p);

            private:
                std::vector<settings::ParamSettings> getBoolParamSettings();
                std::map<std::string, std::function<ofTexture()>> textures_;
                std::map<std::string, std::string> texture_params_;
                std::map<std::string, settings::ParamSettings> params_;
        };
    }
}
