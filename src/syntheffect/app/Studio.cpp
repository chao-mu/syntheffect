#include "syntheffect/app/Studio.h"

namespace syntheffect {
    namespace app {
        void Studio::setup() {
            float insert_pos = 0;

            gui_inputs_panel_ = std::make_shared<ofxDatGui>(insert_pos, 0);
            gui_inputs_panel_->addHeader("Input");
            insert_pos += gui_inputs_panel_->getWidth();

            gui_assets_panel_ = std::make_shared<ofxDatGui>(insert_pos, 0);
            gui_assets_panel_->addHeader("Assets");
            insert_pos += gui_assets_panel_->getWidth();

            gui_pipelines_panel_ = std::make_shared<ofxDatGui>(insert_pos, 0);
            gui_pipelines_panel_->addHeader("Pipelines");
            insert_pos += gui_pipelines_panel_->getWidth();
        }

        void Studio::draw() {
        }

        void Studio::update() {
        }
    }
}
