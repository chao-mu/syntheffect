#pragma once

#include "syntheffect/midi/MidiMessage.h"

namespace syntheffect {
    namespace midi {
        class CmdMicro {
            protected:
                void handleCmdMicroEvent(MidiMessage msg);
                virtual void onCmdMicroLeftOneButtonOn() {};
                virtual void onCmdMicroLeftOneButtonOff() {};
                virtual void onCmdMicroLeftTwoButtonOn() {};
                virtual void onCmdMicroLeftTwoButtonOff() {};
                virtual void onCmdMicroLeftTurnTableSpinRight() {};
                virtual void onCmdMicroLeftTurnTableSpinLeft() {};
                virtual void onCmdMicroRightTurnTableSpinRight() {};
                virtual void onCmdMicroRightTurnTableSpinLeft() {};
                virtual void onCmdMicroLeftPlayPauseButtonOn() {};
                virtual void onCmdMicroLeftPlayPauseButtonOff() {};
                virtual void onCmdMicroRightOneButtonOn() {};
                virtual void onCmdMicroRightOneButtonOff() {};
                virtual void onCmdMicroRightTwoButtonOn() {};
                virtual void onCmdMicroRightTwoButtonOff() {};
                virtual void onCmdMicroLeftLeftFaderSlide(unsigned char v) {};
                virtual void onCmdMicroUnknown(MidiMessage msg) {};
        };
    }
}
