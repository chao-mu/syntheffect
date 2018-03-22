#pragma once

#include "ofMain.h"

#include "RtMidi.h"
#include "syntheffect/filter/Delay.h"
#include "syntheffect/filter/FilterChain.h"
#include "syntheffect/midi/CmdMicro.h"
#include "syntheffect/video/Playlist.h"

namespace syntheffect {
	class ofApp : public ofBaseApp, midi::CmdMicro {
		public:
			ofApp(RtMidiIn* midi_in, std::string playlist_path);
			void setup();
			void update();
			void draw();

			void onCmdMicroLeftLeftFaderSlide(unsigned char v);

		protected:
			void windowResized(int w, int h);

		private:
			filter::Delay delay_filter_;
			filter::FilterChain filters_;
			video::Video video_;
			video::Playlist playlist_;
			RtMidiIn *midi_in_;
			std::string playlist_path_;
	};
}