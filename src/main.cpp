#include <tclap/CmdLine.h>

#include "syntheffect/app/Live.h"
#include "syntheffect/app/Record.h"
#include "syntheffect/app/DrawLoop.h"
#include "syntheffect/patch/PatchBuilder.h"

int main(int argc, const char *argv[]){
    TCLAP::CmdLine cmd("Syntheffect - Magical magic magic");

    TCLAP::SwitchArg liveArg("l", "live", "Live visual effects");
    TCLAP::SwitchArg recordArg("r", "record", "Add visual effects to a video");
    cmd.xorAdd(liveArg, recordArg);

    TCLAP::ValueArg<std::string> inputArg("i", "input", "Input video", true, "", "string");
    cmd.add(inputArg);

    TCLAP::ValueArg<std::string> patchArg("p", "patch", "patch file", false, "patches/default.xml", "string");
    cmd.add(patchArg);

    try {
        cmd.parse(argc, argv);
    } catch (TCLAP::ArgException &e) {
        std::cerr << "error: " << e.error() << " for arg " << e.argId() << std::endl;
    }
 
    if (liveArg.isSet()) {
        ofGLFWWindowSettings settings;
        settings.setGLVersion(3, 3); // OpenGL 3,3 #version 330
        settings.setPosition(ofVec2f(0,0));
        settings.setSize(1280, 720);
        ofCreateWindow(settings);
        ofSetFullscreen(true);
        ofHideCursor();

        auto app = make_shared<syntheffect::app::Live>(patchArg.getValue(), inputArg.getValue());
        ofSetBackgroundColor(0, 0, 0);

        ofRunApp(app);
    } else if (recordArg.isSet()) {
        ofSetupOpenGL(1024, 768, OF_WINDOW);
        ofRunApp(new syntheffect::app::Record(patchArg.getValue(), inputArg.getValue()));
    }

    return 0;
}
