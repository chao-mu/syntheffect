#include "syntheffect/effect/Effect.h"

#include "ofTexture.h"

namespace syntheffect {
    namespace effect {
        Effect::Effect() : float_params_(),  int_params_(), bool_params_() {
            active_ = true;
        }

        bool Effect::isActive() {
            return active_;
        }

        void Effect::toggle() {
            if (isActive()) {
                stop();
            } else {
                start();
            }
        }

        void Effect::start() {
            active_ = true;
        }

        void Effect::stop() {
            active_ = false;
        }

        void Effect::setParam(std::string param, std::function<float()> get) {
            float_params_[param] = get;
        }

        void Effect::setParam(std::string param, std::function<bool()> get) {
            bool_params_[param] = get;
        }

        void Effect::setParam(std::string param, std::function<int()> get) {
            int_params_[param] = get;
        }

        void Effect::setParam(std::string param, std::function<ofTexture()> get) {
            texture_params_[param] = get;
        }
    }
}