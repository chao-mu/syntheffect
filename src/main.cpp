#include <vector>
#include <memory>

#include <tclap/CmdLine.h>

#include "ofFileUtils.h"

#include "syntheffect/app/Live.h"
#include "syntheffect/app/LiveSettings.h"
#include "syntheffect/app/Renderer.h"
#include "syntheffect/patch/PatchBuilder.h"
#include "syntheffect/graphics/Drawable.h"
#include "syntheffect/graphics/Video.h"
#include "syntheffect/graphics/Image.h"
#include "syntheffect/graphics/Webcam.h"

#define IMAGE_EXTS { "jpg", "png", "tiff", "jpeg", "bmp" }

#define DEFAULT_WIDTH 1280
#define DEFAULT_HEIGHT 720

bool check_file(std::string path) {
    if (!std::ifstream(path)) {
        std::cerr << "error: unable to open " << path << std::endl;
        return false;
    }

    return true;
}

bool is_image(std::string path) {
    std::string actual_ext = ofFilePath::getFileExt(path);
    for (std::string img_ext : IMAGE_EXTS) {
        if (actual_ext == img_ext) {
            return true;
        }
    }

    return false;
}

// Consider digits as device numbers for webcams
bool is_webcam(std::string arg) {
    return arg == "%webcam%";
}

int main(int argc, const char *argv[]){
    TCLAP::CmdLine cmd("Syntheffect - Magical magic magic");

    TCLAP::MultiArg<std::string> input_args("i", "input", "Input for a channel chosen by the number of times flag specified previously.", true, "string", cmd);
    TCLAP::ValueArg<std::string> outArg("o", "out", "Output video", false, "", "string", cmd);
    TCLAP::ValueArg<std::string> patchArg("p", "patch", "patch file", false, "patches/default.xml", "string", cmd);
    TCLAP::ValueArg<int> widthArg("w", "width", "width of video", false, DEFAULT_WIDTH, "int", cmd);
    TCLAP::ValueArg<int> heightArg("l", "height", "height of video", false, DEFAULT_HEIGHT, "int", cmd);
    TCLAP::ValueArg<float> volumeArg("v", "volume", "volume between 0 and 1", false, 0, "float", cmd);
    TCLAP::SwitchArg fsArg("f", "fullscreen", "set window to fullscreen", cmd);

    try {
        cmd.parse(argc, argv);
    } catch (TCLAP::ArgException &e) {
        std::cerr << "error: " << e.error() << " for arg " << e.argId() << std::endl;
        return 1;
    }

    std::string patch_path = ofFilePath::getAbsolutePath(patchArg.getValue(), false);

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

    std::vector<std::shared_ptr<syntheffect::graphics::Drawable>> drawables;
    for (std::string path : input_args.getValue()) {
        if (is_webcam(path)) {
            drawables.push_back(make_shared<syntheffect::graphics::Webcam>());
            continue;
        }

        path = ofFilePath::getAbsolutePath(path, false);
        if (!check_file(path)) {
            return 1;
        }

        if (is_image(path)) {
            drawables.push_back(make_shared<syntheffect::graphics::Image>(path));
        } else {
            drawables.push_back(make_shared<syntheffect::graphics::Video>(path, volumeArg.getValue()));
        }
    }


    auto settings = std::make_shared<syntheffect::app::LiveSettings>();
    settings->patch_path = patch_path;
    settings->out_path = out_path;
    settings->recording_height = heightArg.getValue();
    settings->recording_width = widthArg.getValue();
    settings->drawables = drawables;

    auto app = make_shared<syntheffect::app::Live>(settings);
    try {
        ofRunApp(app);
    } catch (std::runtime_error& err) {
        ofLogFatalError() << err.what();
        return 1;
    }

    return 0;
}
