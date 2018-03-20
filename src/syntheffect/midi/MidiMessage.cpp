#include "syntheffect/midi/MidiMessage.h"

#include <vector>
#include <iostream>
#include <cstdint>

namespace syntheffect {
    namespace midi {
        MidiMessage::MidiMessage(std::vector<unsigned char> message) {
            for (unsigned int i=0; i < message.size(); i++) {
                rawMessage.push_back(message[i]);
            }
        }

        MidiType MidiMessage::getType() {
            unsigned char typeByte = rawMessage[0];

            switch(typeByte & 0xf0) {
                case 0x80: return MIDI_TYPE_NOTE_OFF;
                case 0x90: return MIDI_TYPE_NOTE_ON;
                case 0xB0: return MIDI_TYPE_CONTROL;
            }

            return MIDI_TYPE_UNKNOWN;
        }

        // Retrieves the note number. Available for MIDI_TYPE_NOTE_ON/OFF.
        unsigned char MidiMessage::getNote() {
            return rawMessage[1];
        }

        // Retrieves the velocity of a note. Available for MIDI_TYPE_NOTE_ON/OFF.
        unsigned char MidiMessage::getVelocity() {
            return rawMessage[2];
        }

        // Retrieves the function of a control event. Available for MIDI_TYPE_CONTROL.
        unsigned char MidiMessage::getFunction() {
            return rawMessage[1];
        }

        // Retrieves the value of a control event. Available for MIDI_TYPE_CONTROL.
        // Note: Does not take into consideration LSB vs MSB.
        unsigned char MidiMessage::getValue() {
            return rawMessage[2];
        }
    }
}