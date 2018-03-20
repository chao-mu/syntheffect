#pragma once

#include "ofMain.h"
#include "RtMidi.h"
#include "syntheffect/filter/Delay.h"
#include "syntheffect/midi/CmdMicro.h"

namespace syntheffect {
	class ofApp : public ofBaseApp, midi::CmdMicro {
		public:
			ofApp(RtMidiIn* midi_in);
			void setup();
			void update();
			void draw();

			void onCmdMicroLeftLeftFaderSlide(unsigned char v);

		protected:
			RtMidiIn *midi_in_;
			void setupPlane(int w, int h);
			void setupFbo(int w, int h);
			void setupDrawSize(ofFbo fbo);
			void windowResized(int w, int h);

		private:
		  ofVideoPlayer video_player_;
		  ofPlanePrimitive plane_;
		  ofFbo fbo_;
		  filter::Delay delay_filter_;
		  int video_width_;
		  int video_height_;
		  int draw_height_;
		  int draw_width_;
	};
}