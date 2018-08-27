#include "syntheffect/asset/Webcam.h"

#include <stdexcept>

namespace syntheffect {
    namespace asset {
        Webcam::Webcam(std::string path, int device_id) : Asset(path) {
            device_id_ = device_id;
        }

        void Webcam::setup() {
            grabber_.setUseTexture(true);
            grabber_.setDeviceID(device_id_);

            if (!grabber_.initGrabber(1280, 1024)) {
                throw std::runtime_error("Error loading webcam");
            }
        }

        void Webcam::update(float t) {
            grabber_.update();
        }

        void Webcam::draw(float x, float y, float width, float height) {
            grabber_.draw(x, y, width, height);
        }

        bool Webcam::isReady() {
            return grabber_.isInitialized();
        }

        float Webcam::getHeight() {
            return grabber_.getHeight();
        }

        float Webcam::getWidth() {
            return grabber_.getWidth();
        }
    }
}
