#include "syntheffect/midi/CmdMicro.h"

namespace syntheffect {
    namespace midi {
        void CmdMicro::handleCmdMicroEvent(MidiMessage msg) {
            switch(msg.getType()) {
                case MIDI_TYPE_NOTE_ON:
                    switch (msg.getNote()) {
                        case 18:
                            onCmdMicroLeftOneButtonOn();
                            return;
                        case 19:
                            onCmdMicroLeftTwoButtonOn();
                            return;
                        case 23:
                            onCmdMicroLeftPlayPauseButtonOn();
                            return;
                        case 34:
                            onCmdMicroRightOneButtonOn();
                            return;
                        case 36:
                            onCmdMicroRightTwoButtonOn();
                            return;
                        default:
                            onCmdMicroUnknown(msg);
                            return;
                    }
                case MIDI_TYPE_CONTROL:
                    switch (msg.getFunction()) {
                        case 16:
                            onCmdMicroLeftLeftFaderSlide(msg.getValue());
                            return;
                        case 17:
                            if (msg.getValue() > 64) {
                                onCmdMicroLeftTurnTableSpinLeft();
                            } else {
                                onCmdMicroLeftTurnTableSpinRight();
                            }
                            return;
                        case 33:
                            if (msg.getValue() > 64) {
                                onCmdMicroRightTurnTableSpinLeft();
                            } else {
                                onCmdMicroRightTurnTableSpinRight();
                            }
                            return;
                        default:
                            onCmdMicroUnknown(msg);
                            return;
                    }
                default:
                    onCmdMicroUnknown(msg);
                    return;
            }
        }
    }
}
