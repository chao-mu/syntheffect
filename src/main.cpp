#include <tclap/CmdLine.h>

#include "syntheffect/app/Live.h"
#include "syntheffect/app/DrawLoop.h"
#include "syntheffect/patch/PatchBuilder.h"

bool check_file(TCLAP::ValueArg<std::string>& path) {
    if (!std::ifstream(path.getValue())) {
        std::cerr << "error: unable to open value of flag " << path.getFlag() << " which is " << path.getValue();
        return false;
    }

    return true;
}

int main(int argc, const char *argv[]){
    TCLAP::CmdLine cmd("Syntheffect - Magical magic magic");

    TCLAP::ValueArg<std::string> inputArg("i", "input", "Input video", true, "", "string");
    cmd.add(inputArg);
    
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
    
    if (!(check_file(inputArg) || check_file(patchArg))) {
        return 1;
    }
    
    std::string input_path = ofFilePath::getAbsolutePath(inputArg.getValue(), false); 
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

    auto app = make_shared<syntheffect::app::Live>(patch_path, input_path, output_path);
    ofSetBackgroundColor(0, 0, 0);

    ofRunApp(app);

    return 0;
}
