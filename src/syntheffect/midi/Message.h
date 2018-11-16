#ifndef SYNTHEFFECT_MIDI_MESSAGE_H
#define SYNTHEFFECT_MIDI_MESSAGE_H

#include <vector>

namespace syntheffect {
    namespace midi {
        enum MessageType {
            MESSAGE_TYPE_UNKNOWN,
            MESSAGE_TYPE_NOTE_ON,
            MESSAGE_TYPE_NOTE_OFF,
            MESSAGE_TYPE_CONTROL
        };

        class Message {
            public:
                explicit Message(std::vector<unsigned char> message);
                MessageType getType();
                unsigned char getVelocity();
                unsigned char getFunction();
                unsigned char getNote();
                unsigned char getValue();
                unsigned char getChannel();

            private:
                std::vector<unsigned char> message_;
        };
    }
}
#endif
