#include "syntheffect/patch/Patch.h"

#include "ofUtils.h"

#include "syntheffect/filter/Blur.h"
#include "syntheffect/filter/ColorDisplace.h"
#include "syntheffect/filter/Delay.h"
#include "syntheffect/filter/Huerific.h"
#include "syntheffect/filter/Scharr.h"
#include "syntheffect/filter/Sharpen.h"
#include "syntheffect/filter/Sobel.h"
#include "syntheffect/filter/SpinZoom.h"


namespace syntheffect {
    namespace patch {
        Patch::Patch() : filters_() {
        }

        bool Patch::load(std::string path) {
            ofBuffer buf = ofBufferFromFile(path);
            ofBuffer::Lines lines = buf.getLines();
            for (auto line: lines) {
                if (!line.empty()) {
                    maybeAddFilter<filter::Blur>(line, "blur");
                    maybeAddFilter<filter::ColorDisplace>(line, "color_displace");
                    maybeAddFilter<filter::Delay>(line, "delay");
                    maybeAddFilter<filter::Huerific>(line, "huerific");
                    maybeAddFilter<filter::Scharr>(line, "scharr");
                    maybeAddFilter<filter::Sharpen>(line, "sharpen");
                    maybeAddFilter<filter::Sobel>(line, "sobel");
                    maybeAddFilter<filter::SpinZoom>(line, "spin_zoom");
                }
            }

            return true;
        }

        template <typename T>
        void Patch::maybeAddFilter(std::string line, std::string name) {
            if (line == name) {
                shared_ptr<filter::FilterBase> filter = make_shared<T>();
                filter->start();
                filters_.push_back(filter);
            }
        }

        void Patch::draw(graphics::PingPongBuffer& ping_pong) {
            float t = ofGetElapsedTimef();
            for (auto filter: filters_) {
                if (filter->isActive()) {
                    ping_pong.swap();
                    filter->draw(ping_pong, t);
                }
            }
        }
    }
}