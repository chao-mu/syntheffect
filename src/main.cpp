#include <vector>
#include <memory>

#include <tclap/CmdLine.h>

#include "ofFileUtils.h"

#include "syntheffect/app/Live.h"
#include "syntheffect/xml/Parser.h"
#include "syntheffect/settings/ProjectSettings.h"

#define IMAGE_EXTS { "jpg", "png", "tiff", "jpeg", "bmp" }

#define DEFAULT_WIDTH 1280
#define DEFAULT_HEIGHT 720

int main(int argc, const char *argv[]){
    TCLAP::CmdLine cmd("Syntheffect - Magical magic magic");

    TCLAP::ValueArg<std::string> outArg("o", "out", "Output video", false, "", "string", cmd);
    TCLAP::ValueArg<std::string> projectArg("p", "project", "project directory", false, "projects/example/project.xml", "string", cmd);
    TCLAP::ValueArg<float> volumeArg("s", "volume", "volume between 0 and 1", false, 0, "float", cmd);
    TCLAP::SwitchArg fsArg("f", "fullscreen", "set window to fullscreen", cmd);

    try {
        cmd.parse(argc, argv);
    } catch (TCLAP::ArgException &e) {
        std::cerr << "error: " << e.error() << " for arg " << e.argId() << std::endl;
        return 1;
    }

    std::string project_path = ofFilePath::getAbsolutePath(projectArg.getValue(), false);

    std::string out_path = "";
    if (outArg.getValue() != "") {
        out_path = ofFilePath::getAbsolutePath(outArg.getValue(), false);
    }

    ofGLFWWindowSettings win_settings;
    win_settings.setGLVersion(3, 3); // OpenGL 3,3 #version 330
    win_settings.setPosition(ofVec2f(0,0));
    win_settings.setSize(1280, 720);
    if (fsArg.getValue()) {
        win_settings.windowMode = OF_FULLSCREEN;
    }

    ofCreateWindow(win_settings);

    syntheffect::xml::Parser p;
    std::string settings_path = projectArg.getValue();
    if (!ofFilePath::isAbsolute(settings_path)) {
        settings_path = ofFilePath::join(ofFilePath::getCurrentWorkingDirectory(), settings_path);
    }

    syntheffect::settings::ProjectSettings settings = p.parseProject(settings_path);
    settings.out_path = out_path;

    auto app = make_shared<syntheffect::app::Live>(settings);
    try {
        ofRunApp(app);
    } catch (std::runtime_error& err) {
        ofLogFatalError() << err.what();
        return 1;
    }

    return 0;
}
