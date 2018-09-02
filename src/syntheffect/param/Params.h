#ifndef SYNTHEFFECT_PARAM_PARAMS_H
#define SYNTHEFFECT_PARAM_PARAMS_H

#pragma once

#include <map>
#include <variant>

#include "ofTexture.h"

#include "syntheffect/param/Param.h"

namespace syntheffect {
    namespace param {
        class Params {
            public:
                void set(Param p);
                void setTexture(const std::string& name, std::function<ofTexture()> v);
                void setTexture(const std::string& name, const std::string& target);

                const Param& at(const std::string& name) const;
                const Param& resolveParent(const Param& p) const;
                const ParamCast& resolveCast(const Param& p, int resolutions=0) const;
                float resolveValue(const Param& p, int resolutions=0) const;

                bool getBool(const std::string& name) const;
                int getInt(const std::string& name) const;
                float getFloat(const std::string& name) const;
                ofTexture getTexture(const std::string& name) const;

                bool getBool(const std::string& name, bool def) const;
                int getInt(const std::string& name, int def) const;
                float getFloat(const std::string& name, float def) const;
                ofTexture getTexture(const std::string& name, ofTexture def) const;

                std::map<std::string, bool> getBools() const;
                std::map<std::string, int> getInts() const;
                std::map<std::string, float> getFloats() const;
                std::map<std::string, ofTexture> getTextures() const;

                void copyTo(Params& p) const;

                bool exists(const std::string& name) const;

                bool isInt(const Param& p) const;
                bool isBool(const Param& p) const;
                bool isFloat(const Param& p) const;

            private:
                std::vector<Param> getBoolParamSettings() const;

                std::map<std::string, std::function<ofTexture()>> textures_;
                std::map<std::string, std::string> texture_params_;
                std::map<std::string, Param> params_;
        };
    }
}

#endif
