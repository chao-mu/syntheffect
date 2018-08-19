#pragma once

#include <map>
#include <variant>

#include "ofTexture.h"

#include "syntheffect/settings/ParamSettings.h"

namespace syntheffect {
    namespace param {
        class Params {
            public:
                void set(const settings::ParamSettings p);
                void setTexture(std::string name, std::function<ofTexture()> v);
                void setTexture(std::string name, std::string target);

                const settings::ParamSettings& at(std::string name) const;
                const settings::ParamSettings& resolveParent(const settings::ParamSettings& p) const;
                const settings::ParamCast& resolveCast(const settings::ParamSettings& p, int resolutions=0) const;
                float resolveValue(const settings::ParamSettings& p, int resolutions=0) const;

                bool getBool(std::string name) const;
                int getInt(std::string name) const;
                float getFloat(std::string name) const;
                ofTexture getTexture(std::string name) const;

                bool getBool(std::string name, bool def) const;
                int getInt(std::string name, int def) const;
                float getFloat(std::string name, float def) const;
                ofTexture getTexture(std::string name, ofTexture def) const;

                std::map<std::string, bool> getBools() const;
                std::map<std::string, int> getInts() const;
                std::map<std::string, float> getFloats() const;
                std::map<std::string, ofTexture> getTextures() const;

                void copyTo(Params& p) const;

                bool exists(std::string name) const;

                bool isInt(const settings::ParamSettings& p) const;
                bool isBool(const settings::ParamSettings& p) const;
                bool isFloat(const settings::ParamSettings& p) const;

            private:
                std::vector<settings::ParamSettings> getBoolParamSettings() const;

                std::map<std::string, std::function<ofTexture()>> textures_;
                std::map<std::string, std::string> texture_params_;
                std::map<std::string, settings::ParamSettings> params_;
        };
    }
}
