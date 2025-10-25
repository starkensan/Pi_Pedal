#include <Arduino.h>
#include <DebugLog.h>
#include <config.h>

#include <EepromStrage.h>

enum MidiMode
{
    MIDI_MODE_OFF = 0,
    MIDI_MODE_CC,
    MIDI_MODE_PC_NEXT,
    MIDI_MODE_PC_BACK
};

struct PedalMidiSetting
{
    uint8_t pedalNum;
    MidiMode midiMode;
    int ccNumber;
};

PedalMidiSetting setting = {1, MIDI_MODE_CC, 74};

EepromStrage es;
PedalMidiSetting readSetting;

void setup() {
    Serial.begin(115200);
}

void loop() {

}