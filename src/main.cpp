#include "ofMain.h"

#include "syntheffect/ofApp.h"
#include "RtMidi.h"

int main(int argc, const char *argv[]){
    shared_ptr<RtMidiIn> midi_in = make_shared<RtMidiIn>();

    if (argc != 2) {
        std::cout << "Usage: syntheffect path/to/playlist.m3u\n";
        return -1;
    }

    std::string playlist_path = argv[1];
 
    // Check available ports.
    unsigned int nPorts = midi_in->getPortCount();
    if (nPorts == 0) {
        std::cout << "No MIDI ports available!\n";
        return -1;
    }
    midi_in->openPort(0);

    ofGLFWWindowSettings settings;
    settings.setGLVersion(3, 3); // OpenGL 3,3 #version 330
    settings.setPosition(ofVec2f(0,0));
    settings.width = 1280;
    settings.height = 720;
    ofCreateWindow(settings);
    ofSetFullscreen(true);
    ofHideCursor();

    shared_ptr<syntheffect::ofApp> app(new syntheffect::ofApp(midi_in, playlist_path));
    ofSetBackgroundColor(0, 0, 0);

    ofRunApp(app);

    return 0;
}