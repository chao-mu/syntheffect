#ifndef SYNTHEFFECT_MIDI_CONTROL
#define SYNTHEFFECT_MIDI_CONTROL

#include <string>

namespace syntheffect {
    namespace midi {
        enum ControlType {
            CONTROL_TYPE_BUTTON,
            CONTROL_TYPE_FADER,
            CONTROL_TYPE_UNKNOWN
        };

        struct Control {
            std::string name = "";
            ControlType type = CONTROL_TYPE_UNKNOWN;
            bool pressed = false;
            unsigned char value = 0;
            unsigned char low = 0;
            unsigned char high = 0;
            unsigned char function = 0;
            unsigned char channel = 0;
        };
    }
}
#endif
