#include <vector>
#include <memory>

#include <tclap/CmdLine.h>

#include "ofFileUtils.h"

#include "syntheffect/app/Live.h"
#include "syntheffect/app/Renderer.h"
#include "syntheffect/patch/PatchBuilder.h"
#include "syntheffect/graphics/Drawable.h"
#include "syntheffect/graphics/Video.h"

#include "syntheffect/strings.h"

#define IMAGE_EXTS { "jpg", "png", "tiff", "jpeg", "bmp" }


bool check_file(std::string path) {
    if (!std::ifstream(path)) {
        std::cerr << "error: unable to open " << path;
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

int main(int argc, const char *argv[]){
    TCLAP::CmdLine cmd("Syntheffect - Magical magic magic");

    TCLAP::MultiArg<std::string> input_args("i", "input", "Input for a channel chosen by the number of times flag specified previously.", true, "string");
    cmd.add(input_args);

    TCLAP::ValueArg<std::string> outputArg("o", "output", "Output video", false, "", "string");
    cmd.add(outputArg);

    TCLAP::ValueArg<std::string> patchArg("p", "patch", "patch file", false, "patches/default.xml", "string");
    cmd.add(patchArg);

    try {
        cmd.parse(argc, argv);
    } catch (TCLAP::ArgException &e) {
        std::cerr << "error: " << e.error() << " for arg " << e.argId() << std::endl;
        return 1;
    }

    if (!check_file(patchArg.getValue())) {
        return 1;
    }

    std::string patch_path = ofFilePath::getAbsolutePath(patchArg.getValue(), false);

    std::string output_path = "";
    if (outputArg.getValue() != "") {
        output_path = ofFilePath::getAbsolutePath(outputArg.getValue(), false);
    }

    ofGLFWWindowSettings settings;
    settings.setGLVersion(3, 3); // OpenGL 3,3 #version 330
    settings.setPosition(ofVec2f(0,0));
    settings.setSize(1280, 720);
    ofCreateWindow(settings);
    ofSetFullscreen(true);
    ofHideCursor();

    std::vector<std::shared_ptr<syntheffect::graphics::Drawable>> drawables;

    for (std::string path : input_args.getValue()) {
        path = ofFilePath::getAbsolutePath(path, false);
        if (!check_file(path)) {
            return 1;
        }

        if (is_image(path)) {
            //drawables.push_back(make_shared<syntheffect::graphics::Image>(path));
        } else {
            drawables.push_back(make_shared<syntheffect::graphics::Video>(path));
        }
    }


    auto app = make_shared<syntheffect::app::Live>(patch_path, drawables, output_path);
    ofSetBackgroundColor(0, 0, 0);

    try {
        ofRunApp(app);
    } catch (std::runtime_error& err) {
        ofLogFatalError() << err.what();
        return 1;
    }

    return 0;
}
