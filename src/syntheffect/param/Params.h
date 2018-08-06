#pragma once

#include <map>
#include <variant>

#include "ofTexture.h"

#include "syntheffect/param/ParamAccessors.h"

namespace syntheffect {
    namespace param {
        class Params : public ParamAccessors {
            public:
                void setParamAxis(std::string name, std::string which, bool absolute, float low, float high);

                void setParamAxisPositive(std::string which, std::string name);
                void setParamAxisNegative(std::string which, std::string name);

                void setParamWaveCos(std::string name, float shift, float freq, float low, float high);
                void setParamWavePerlin(std::string name, float shift, float freq, float low, float high);
                void setParamWaveSin(std::string name, float shift, float freq, float low, float hight);

                void setParamPressed(std::string which, std::string name);
                void setParamPressedTime(std::string which, std::string name, float offset, float speed);

                void set(std::string name, std::function<bool(ParamAccessors&)> v) { bool_params_[name] = v; }
                void set(std::string name, std::function<int(ParamAccessors&)> v) { int_params_[name] = v; }
                void set(std::string name, std::function<float(ParamAccessors&)> v) { float_params_[name] = v; }
                void set(std::string name, std::function<ofTexture(ParamAccessors&)> v) { texture_params_[name] = v; }

                void set(std::string name, bool v) { bool_params_[name] = [v](ParamAccessors&){ return v; }; }
                void set(std::string name, int v) { int_params_[name] = [v](ParamAccessors&){ return v; }; }
                void set(std::string name, float v) { float_params_[name] = [v](ParamAccessors&){ return v; }; }
                void set(std::string name, ofTexture v) { texture_params_[name] = [v](ParamAccessors&){ return v; }; }

                bool getBool(std::string name) override { return bool_params_[name](*this); }
                int getInt(std::string name) override { return int_params_[name](*this); }
                float getFloat(std::string name) override { return float_params_[name](*this); }
                ofTexture getTexture(std::string name) override { return texture_params_[name](*this); }

                bool getBool(std::string name, bool def) override;
                int getInt(std::string name, int def) override;
                float getFloat(std::string name, float def) override;
                ofTexture getTexture(std::string name, ofTexture def) override;

                std::map<std::string, bool> getBools();
                std::map<std::string, int> getInts();
                std::map<std::string, float> getFloats();
                std::map<std::string, ofTexture> getTextures();

                void copyTo(Params& p);

            private:
                std::map<std::string, std::function<bool(ParamAccessors&)>> bool_params_;
                std::map<std::string, std::function<int(ParamAccessors&)>> int_params_;
                std::map<std::string, std::function<float(ParamAccessors&)>> float_params_;
                std::map<std::string, std::function<ofTexture(ParamAccessors&)>> texture_params_;
        };
    }
}
