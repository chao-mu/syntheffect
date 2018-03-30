#pragma once

#include "ofMain.h"

#include "RtMidi.h"
#include "syntheffect/patch/Patch.h"
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
			shared_ptr<patch::Patch> patch_;
			shared_ptr<RtMidiIn> midi_in_;

			video::Video video_;
			video::Playlist playlist_;
			std::string playlist_path_;
	};
}