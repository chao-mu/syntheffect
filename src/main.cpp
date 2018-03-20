#include "ofMain.h"
#include "syntheffect/ofApp.h"
#include "RtMidi.h"

//========================================================================
int main() {
    RtMidiIn *midi_in = new RtMidiIn();
 
    // Check available ports.
    unsigned int nPorts = midi_in->getPortCount();
    if (nPorts == 0) {
        std::cout << "No MIDI ports available!\n";
        delete midi_in;
        return 1;
    }
    midi_in->openPort(0);

    ofGLFWWindowSettings showSettings;
    showSettings.setGLVersion(3, 3); // OpenGL 3,3 #version 330
    showSettings.setPosition(ofVec2f(0,0));
    showSettings.width = 1280;
    showSettings.height = 720;
    shared_ptr<ofAppBaseWindow> showWindow = ofCreateWindow(showSettings);

    shared_ptr<syntheffect::ofApp> app(new syntheffect::ofApp(midi_in));
    ofSetBackgroundColor(ofColor::black);

    ofRunApp(app);

    delete midi_in;

    return 0;
}