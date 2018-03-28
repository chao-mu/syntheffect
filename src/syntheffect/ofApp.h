#pragma once

#include "ofMain.h"

#include "RtMidi.h"
#include "syntheffect/filter/Delay.h"
#include "syntheffect/filter/Sobel.h"
#include "syntheffect/filter/Scharr.h"
#include "syntheffect/filter/Huerific.h"
#include "syntheffect/filter/SpinZoom.h"
#include "syntheffect/filter/ColorDisplace.h"
#include "syntheffect/midi/CmdMicro.h"
#include "syntheffect/video/Playlist.h"

namespace syntheffect {
	class ofApp : public ofBaseApp, midi::CmdMicro {
		public:
			ofApp(shared_ptr<RtMidiIn> midi_in, std::string playlist_path);
			void setup();
			void update();
			void draw();

			void onCmdMicroLeftLeftFaderSlide(unsigned char v);

		protected:
			void windowResized(int w, int h);
			void keyPressed(int c);

		private:
			shared_ptr<filter::Delay> delay_filter_;
			shared_ptr<filter::Huerific> hue_filter_;
			shared_ptr<filter::SpinZoom> spin_zoom_filter_;
			shared_ptr<filter::ColorDisplace> color_displace_filter_;
			shared_ptr<filter::Sobel> sobel_filter_;
			shared_ptr<filter::Scharr> scharr_filter_;

			vector<shared_ptr<filter::FilterBase>> filters_;

			shared_ptr<RtMidiIn> midi_in_;

			video::Video video_;
			video::Playlist playlist_;
			std::string playlist_path_;
	};
}