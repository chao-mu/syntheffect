#pragma once

#include "syntheffect/settings/AssetGroupSettings.h"

#include "syntheffect/param/Params.h"

#include "syntheffect/graphics/Drawable.h"
#include "syntheffect/graphics/Image.h"
#include "syntheffect/graphics/Video.h"
#include "syntheffect/graphics/PingPongBufferMap.h"

namespace syntheffect {
    namespace manager {
        class AssetManager {
            public:
                AssetManager(int width, int height);

                void setAssets(const std::vector<settings::AssetGroupSettings>& asset_groups);

                void update(float t, param::Params& params);
                void setup();

                bool isReady();
                bool isFinished();

                int getWidth();
                int getHeight();

                graphics::PingPongBufferMap& getChannels();

            private:
                std::map<std::string, std::shared_ptr<graphics::Drawable>> active_drawables_;
                std::map<std::string, std::vector<std::shared_ptr<graphics::Drawable>>> triggerable_drawables_;
                std::map<std::shared_ptr<graphics::Drawable>, std::string> drawable_names_;

                graphics::PingPongBufferMap channels_;

                std::string getLastName(std::string buf_name);
                std::map<std::string, std::string> triggers_;
        };
    }
}
