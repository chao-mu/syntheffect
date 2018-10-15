#include <vector>
#include <memory>

#include <tclap/CmdLine.h>

#include "ofFileUtils.h"
#include "ofMain.h"

#include "syntheffect/app/Live.h"
#include "syntheffect/app/Studio.h"
#include "syntheffect/xml/Parser.h"
#include "syntheffect/settings/ProjectSettings.h"

#define IMAGE_EXTS { "jpg", "png", "tiff", "jpeg", "bmp" }

#define DEFAULT_WIDTH 1280
#define DEFAULT_HEIGHT 720

int main(int argc, const char *argv[]){
    TCLAP::CmdLine cmd("Syntheffect - Magical magic magic");

    TCLAP::ValueArg<std::string> outArg("o", "out", "Output video", false, "", "string", cmd);
    TCLAP::ValueArg<std::string> projectArg("p", "project", "project path", true, "", "string", cmd);
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

    ofGLFWWindowSettings win_settings;
    win_settings.setGLVersion(3, 3); // OpenGL 3,3 #version 330
    win_settings.setPosition(ofVec2f(0,0));
    win_settings.setSize(1280, 720);
    if (fsArg.getValue()) {
        win_settings.windowMode = OF_FULLSCREEN;
    }

    auto display_window = ofCreateWindow(win_settings);

    syntheffect::xml::Parser p;
    std::string settings_path = projectArg.getValue();
    if (!ofFilePath::isAbsolute(settings_path)) {
        settings_path = ofFilePath::join(ofFilePath::getCurrentWorkingDirectory(), settings_path);
    }

    syntheffect::settings::ProjectSettings settings = p.parseProject(settings_path);
    settings.out_path = out_path;

    auto app = std::make_shared<syntheffect::app::Live>(settings);
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
