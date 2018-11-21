#ifndef SYNTHEFFECT_MIDI_DEVICE
#define SYNTHEFFECT_MIDI_DEVICE

#include <string>
#include <memory>
#include <mutex>
#include <map>
#include <thread>
#include <atomic>
#include <regex>

#include "RtMidi.h"

#include "syntheffect/midi/Control.h"
#include "syntheffect/rack/Module.h"

namespace syntheffect {
    namespace midi {
        class Device : public rack::Module {
            public:
                Device(const std::string& id, const std::string& path);

                void setup(int width, int height, int internal_format, const std::string& workspace_dir) override;
                void stop() override;
                void update(float t) override;
                void start() override;
                const std::string getType() override;

                static const std::string getModuleType();

            private:
                void load(const std::string& path);
                void loop();

                std::string path_;
                std::regex name_re_;
                std::mutex controls_mutex_;
                std::string port_name_;

                std::shared_ptr<RtMidiIn> midi_in_;
                std::map<std::string, Control> controls_;
                std::thread thread_;
                std::atomic<bool> running_;
        };
    }
}
#endif
