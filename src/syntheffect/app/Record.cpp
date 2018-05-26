#include "syntheffect/app/Record.h"

namespace syntheffect {
    namespace app {
        Record::Record(std::string patch_path, std::string video_path)
                : ofBaseApp(),
                draw_loop_(std::make_shared<DrawLoop>()) {
            patch_path_ = patch_path;
            video_path_ = video_path;
            last_frame_ = 0;
            t_ = 0;
        }

        void Record::setup() {
            if (!draw_loop_->load(patch_path_, video_path_)) {
                ofLogFatalError() << "Failed to load draw loop!";
                ofExit();
	    }


            vid_recorder_.setVideoCodec("libx265");
            vid_recorder_.setVideoBitrate("8000k");

            vid_recorder_.setup(
                "vid-out-" + ofGetTimestampString() + ".mp4",
                draw_loop_->getWidth(),
                draw_loop_->getHeight(),
                draw_loop_->getFPS()
            );

            vid_recorder_.start();

            t_increment_ = 1.0 / draw_loop_->getFPS();

        }

        void Record::update() {
            auto params = std::make_shared<graphics::Params>();
            if (!draw_loop_->update(params, t_)) {
                ofExit();
            }

            if (!draw_loop_->isDrawable()) {
                ofLogNotice() << "not drawing...";
                return;
            }


            ofPixels pixels;
            draw_loop_->channels->get(CHANNEL_OUT)->drawable()->readToPixels(pixels);
            vid_recorder_.addFrame(pixels);

            t_ += t_increment_;

            draw_loop_->video->nextFrame();
            ofLogNotice("Record", "on its own: %d", last_frame_++);
        }
    }
}
