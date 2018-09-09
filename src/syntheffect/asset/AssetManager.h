#ifndef SYNTHEFFECT_ASSET_ASSETMANAGER_H
#define SYNTHEFFECT_ASSET_ASSETMANAGER_H

#pragma once

#include <set>
#include <map>
#include <vector>
#include <memory>

#include "ofxSoundObject.h"

#include "syntheffect/asset/Asset.h"
#include "syntheffect/asset/Drawable.h"
#include "syntheffect/asset/Audio.h"
#include "syntheffect/asset/AudioAnalyzer.h"

namespace syntheffect {
    namespace asset {
        class AssetManager {
            public:
                void update(float t);

                bool isReady();
                bool isFinished();

                void addDrawable(std::shared_ptr<Drawable> asset);
                void addAudio(std::shared_ptr<Audio> asset);

                void activateGroup(const std::string& group);
                void activateAsset(const std::string& asset_id);
                void shuffleActiveStack(const std::string& stack);
                void prevStackAsset(const std::string& stack);
                void nextStackAsset(const std::string& stack);
                void reorderActiveStack(const std::string& stack, std::function<void(std::vector<std::string>&)> f);

                void copyParamsTo(param::Params& p);

                std::vector<std::shared_ptr<Drawable>> getDrawables();
                std::map<std::string, std::string> getStackToAsset();

                ofBaseSoundOutput& getSoundOutput();

            private:
                void addAsset(std::shared_ptr<Asset> asset);

                std::map<std::string, std::shared_ptr<Asset>> assets_by_id_;
                std::map<std::string, std::shared_ptr<Drawable>> drawables_by_id_;
                std::map<std::string, std::shared_ptr<Audio>> audio_by_id_;
                std::map<std::string, std::map<std::string, std::vector<std::string>>> assets_by_group_and_stack_;
                std::map<std::string, std::vector<std::string>> assets_by_stack_;
                std::map<std::string, std::vector<std::string>> assets_by_group_;

                // stack to asset
                std::map<std::string, std::string> active_assets_;

                ofxSoundOutput sound_output_;
                AudioAnalyzer sound_analyzer_;
        };
    }
}

#endif
