#include "syntheffect/video/Playlist.h"

#include "ofxM3U.h"
#include "ofUtils.h"

#include <stdexcept>
#include <vector>

namespace syntheffect {
    namespace video {
        void Playlist::load(std::string path) {
            position_ = 0;
            filenames_.clear();

            ofxM3U playlist;
            playlist.load(path);
            std::vector<M3UItem> items = playlist.getItems();
            if (items.size() < 1) {
                throw std::runtime_error("Can not load empty playlist!");
            }
            
            std::string file;
            for (auto const& item: items) {
                file = item.file;
                ofStringReplace(file, "%20", " ");
                ofStringReplace(file, "file://", "");
                filenames_.push_back(file);
            }
        }

        video::Video Playlist::next() {
            Video video;

            if (!video.load(filenames_[position_])) {
                throw std::runtime_error("Unable to load video.");
            }

            position_ = (position_ + 1) % filenames_.size();

            return video;
        }
    }
}