#include <vector>
#include <memory>

#include <tclap/CmdLine.h>

#include "ofFileUtils.h"
#include "ofMain.h"

#include "syntheffect/app/Live.h"
#include "syntheffect/app/Studio.h"

int main(int argc, const char *argv[]){
    TCLAP::CmdLine cmd("Syntheffect - Magical magic magic");

    TCLAP::ValueArg<std::string> outArg("o", "out", "Output video", false, "", "string", cmd);
    TCLAP::ValueArg<std::string> projectArg("p", "project", "path to rack file", true, "", "string", cmd);
    TCLAP::ValueArg<std::string> modulesArg("m", "modules", "path to modules repository", true, "", "string", cmd);
    TCLAP::SwitchArg fsArg("f", "fullscreen", "set window to fullscreen", cmd);
    TCLAP::SwitchArg studioArg("s", "studio", "open project studio", cmd);

    try {
        cmd.parse(argc, argv);
    } catch (TCLAP::ArgException &e) {
        std::cerr << "error: " << e.error() << " for arg " << e.argId() << std::endl;
        return 1;
    }

    std::string out_path;
    if (!outArg.getValue().empty()) {
        out_path = ofFilePath::getAbsolutePath(outArg.getValue(), false);
    }

    std::string rack_path = ofFilePath::getAbsolutePath(projectArg.getValue(), false);
    std::string modules_dir = ofFilePath::getAbsolutePath(modulesArg.getValue(), false);

    ofGLFWWindowSettings win_settings;
    win_settings.setGLVersion(3, 3); // OpenGL 3,3 #version 330
    win_settings.setPosition(ofVec2f(0,0));
    win_settings.setSize(1280, 720);
    if (fsArg.getValue()) {
        win_settings.windowMode = OF_FULLSCREEN;
    }

    auto display_window = ofCreateWindow(win_settings);

    auto app = std::make_shared<syntheffect::app::Live>(rack_path, modules_dir, out_path);
    ofRunApp(display_window, app);

    if (studioArg.getValue()) {
        win_settings.setPosition(ofVec2f(100,0));
        win_settings.windowMode = OF_WINDOW;
        auto studio_window = ofCreateWindow(win_settings);
        //ofRunApp(studio_window, std::make_shared<syntheffect::app::Studio>());
    }

    ofRunMainLoop();

    return 0;
}
