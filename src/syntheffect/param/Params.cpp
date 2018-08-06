#include "syntheffect/param/Params.h"

#include <map>

#include "ofMath.h"
#include "ofUtils.h"
#include "ofLog.h"

namespace syntheffect {
    namespace param {

        std::map<std::string, bool> Params::getBools() {
            std::map<std::string, bool> bools;
            for (auto const& kv : bool_params_) {
                bools[kv.first] = getBool(kv.first);
            }

            return bools;
        }

        std::map<std::string, int> Params::getInts() {
            std::map<std::string, int> ints;
            for (auto const& kv : int_params_) {
                ints[kv.first] = getInt(kv.first);
            }

            return ints;
        }

        std::map<std::string, float> Params::getFloats() {
            std::map<std::string, float> floats;
            for (auto const& kv : float_params_) {
                floats[kv.first] = getFloat(kv.first);
            }

            return floats;
        }

        std::map<std::string, ofTexture> Params::getTextures() {
            std::map<std::string, ofTexture> textures;
            for (auto const& kv : texture_params_) {
                textures[kv.first] = getTexture(kv.first);
            }

            return textures;
        }

        void Params::setParamAxisPositive(std::string name, std::string which) {
            bool_params_[name] = [which](ParamAccessors& p) {
                return p.getFloat(which) > 0;
            };
        }

        void Params::setParamAxisNegative(std::string name, std::string which) {
            bool_params_[name] = [which](ParamAccessors& p) {
                return p.getFloat(which) < 0;
            };
        }

        bool Params::getBool(std::string name, bool def) {
            if (bool_params_.count(name) > 0) {
                return getBool(name);
            }

            return def;
        }

        int Params::getInt(std::string name, int def) {
            if (int_params_.count(name) > 0) {
                return getInt(name);
            }

            return def;
        }


        float Params::getFloat(std::string name, float def) {
            if (float_params_.count(name) > 0) {
                return getFloat(name);
            }

            return def;
        }


        ofTexture Params::getTexture(std::string name, ofTexture def) {
            if (texture_params_.count(name) > 0) {
                return getTexture(name);
            }

            return def;
        }

        void Params::setParamAxis(std::string name, std::string which, bool absolute, float low, float high) {
            float_params_[name] = [which, low, high, absolute](ParamAccessors& p) {
                float v = p.getFloat(which);

                v = ofMap(v, -1, 1, low, high);
                if (absolute) {
                    v = fabs(v);
                }


                return v;
            };
        }

        void Params::setParamPressed(std::string name, std::string which) {
            bool_params_[name] = [which](ParamAccessors& p) {
                return p.getBool(which);
            };
        }

        void Params::setParamWaveSin(std::string name, float shift, float freq, float low, float high) {
            float_params_[name] = [shift, freq, low, high](ParamAccessors& p) {
                float v = sin(p.getFloat("time") * freq + shift);
                return ofMap(v, -1, 1, low, high);
            };
        }

        void Params::setParamWaveCos(std::string name, float shift, float freq, float low, float high) {
            float_params_[name] = [shift, freq, low, high](ParamAccessors& p) {
                float v = cos(p.getFloat("time") * freq + shift);
                return ofMap(v, -1, 1, low, high);
            };
        }


        void Params::setParamWavePerlin(std::string name, float shift, float freq, float low, float high) {
            float_params_[name] = [shift, freq, low, high](ParamAccessors& p) {
                float v = ofNoise(p.getFloat("time") * freq + shift);
                return ofMap(v, 0, 1, low, high);
            };
        }


        void Params::setParamPressedTime(std::string name, std::string which, float offset, float speed) {
            float_params_[name] = [which, offset, speed](ParamAccessors& p) {
                return speed * (p.getFloat("time") - p.getFloat(which + "_pressed_at") + offset);
            };
        }

        void Params::copyTo(Params& p) {
            for (const auto& kv : float_params_) {
                p.set(kv.first, kv.second);
            }

            for (const auto& kv : bool_params_) {
                p.set(kv.first, kv.second);
            }

            for (const auto& kv : int_params_) {
                p.set(kv.first, kv.second);
            }

            for (const auto& kv : texture_params_) {
                p.set(kv.first, kv.second);
            }
        }
    }
}
