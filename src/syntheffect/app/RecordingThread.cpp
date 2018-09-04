#include "syntheffect/app/RecordingThread.h"

#include <stdexcept>

#include "ofThread.h"

namespace syntheffect {
    namespace app {
        void RecordingThread::setup(const std::string path, int width, int height) {
            recording_buf_.allocate(width, height, GL_RGB8);
            path_ = path;
        }

        int RecordingThread::getHeight() {
            return recording_buf_.getHeight();
        }

        int RecordingThread::getWidth() {
            return recording_buf_.getWidth();
        }

        void RecordingThread::threadedFunction() {
            while (isThreadRunning() || !pending.empty()) {
                if (!pending.empty()) {
                    work();
                }
            }
        }

        void RecordingThread::fboBegin() {
            recording_buf_.begin();
        }

        void RecordingThread::fboEnd() {
            recording_buf_.end();
        }

        void RecordingThread::push() {
            ofPixels pixels;
            recording_buf_.readToPixels(pixels);
            pending.push(pixels);
        }

        void RecordingThread::work() {
            ofFile file;
            if (!file.open(path_, ofFile::Append)) {
                throw std::runtime_error("Unable to open recording output file with path: " + path_);
            }

            ofPixels pixels = pending.front();

            for (size_t i = 0; i < pixels.size(); i++) {
                file << pixels[i];
            }

            file.close();

            pending.pop();
            frame_count_++;
        }
    }
}
