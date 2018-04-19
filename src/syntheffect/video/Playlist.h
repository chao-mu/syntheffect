#pragma once

#include <string>
#include <vector>

#include "syntheffect/video/Video.h"

namespace syntheffect {
    namespace video {
        class Playlist {
            public:
                void load(std::string path);
                shared_ptr<Video> next();

            private:
                std::vector<std::string> filenames_;
                unsigned int position_;
        };
    }
}