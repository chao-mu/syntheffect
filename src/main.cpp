#include <vector>
#include <memory>

#include <tclap/CmdLine.h>

#include "ofFileUtils.h"

#include "syntheffect/app/Live.h"
#include "syntheffect/app/LiveSettings.h"
#include "syntheffect/graphics/Drawable.h"
#include "syntheffect/graphics/Video.h"
#include "syntheffect/graphics/Image.h"
#include "syntheffect/graphics/Webcam.h"

#define IMAGE_EXTS { "jpg", "png", "tiff", "jpeg", "bmp" }

#define DEFAULT_WIDTH 1280
#define DEFAULT_HEIGHT 720
#define PIPELINES_CONFIG "pipelines.xml"
#define ASSETS_CONFIG "assets.xml"

int main(int argc, const char *argv[]){
    TCLAP::CmdLine cmd("Syntheffect - Magical magic magic");

    TCLAP::ValueArg<std::string> outArg("o", "out", "Output video", false, "", "string", cmd);
    TCLAP::ValueArg<std::string> projectArg("p", "project", "project directory", false, "projects/example", "string", cmd);
    TCLAP::ValueArg<int> widthArg("w", "width", "width of video", false, DEFAULT_WIDTH, "int", cmd);
    TCLAP::ValueArg<int> heightArg("l", "height", "height of video", false, DEFAULT_HEIGHT, "int", cmd);
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

    auto settings = std::make_shared<syntheffect::app::LiveSettings>();
    settings->pipelines_path = project_path + "/" + PIPELINES_CONFIG;
    settings->assets_path = project_path + "/" + ASSETS_CONFIG;
    settings->out_path = out_path;
    settings->recording_height = heightArg.getValue();
    settings->recording_width = widthArg.getValue();

    auto app = make_shared<syntheffect::app::Live>(settings);
    try {
        ofRunApp(app);
    } catch (std::runtime_error& err) {
        ofLogFatalError() << err.what();
        return 1;
    }

    return 0;
}
