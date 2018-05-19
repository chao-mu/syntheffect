#include "syntheffect/app/Record.h"

#include "ofxVideoRecorder.h"

#define FPS 30

namespace syntheffect {
    namespace app {
        int Record::run(std::string video_path) {
            /*
            ofxVideoRecorder vid_recorder;

            vid_recorder.setVideoCodec("libx265");
            vid_recorder.setVideoBitrate("5000k");
            vid_recorder.setAudioCodec("mp3");
            vid_recorder.setAudioBitrate("192k");

            vid_recorder.setup(
                "vid-out-" + ofGetTimestampString() + ".mp4",
                video_->getWidth(),
                video_->getHeight(),
                FPS,
                sample_rate,
                channels
            );

            vid_recorder.start();

            ofPixels pixels;
            channels_->get(CHANNEL_OUT)->drawable()->readToPixels(pixels);
            vid_recorder_.addFrame(pixels);
            */

            return 0;
        }
    }
}