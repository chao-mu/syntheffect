#include "syntheffect/asset/Asset.h"

namespace syntheffect {
    namespace asset {
        Asset::Asset(const std::string id) : id_(id), active_(false) {
        }

        bool Asset::isReady() {
            return true;
        }

        void Asset::setup() {
        }

        void Asset::update(float) {
        }

        bool Asset::isFinished() {
            return false;
        }

        void Asset::restart() {
        }

        void Asset::play() {
        }

        void Asset::pause() {
        }

        std::string Asset::getID() {
            return id_;
        }

        void Asset::setID(const std::string id) {
            id_ = id;
        }

        bool Asset::isActive() {
            return active_;
        }

        void Asset::setActive(bool active) {
            active_ = active;
        }

        std::string Asset::getStack() {
            return stack_;
        }

        void Asset::setStack(const std::string stack) {
            stack_ = stack;
        }


        std::string Asset::getGroup() {
            return group_;
        }

        void Asset::setGroup(const std::string group) {
            group_ = group;
        }
    }
}
