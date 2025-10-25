#ifndef USBMIDI_h
#define USBMIDI_h

#include <Arduino.h>
#include <DebugLog.h>
#include <Adafruit_TinyUSB.h>
#include <MIDI.h>

class USBMIDI {
public:
    USBMIDI();

    void begin(String device_name = "PiPedal");
    void sendControlChange(uint8_t channel, uint8_t control, uint8_t value);
    void sendProgramChange(uint8_t channel, uint8_t program);

private:


    // 1. TinyUSB MIDI デバイス本体 (外側で usb_midi を使ってるならここで持つ)
    Adafruit_USBD_MIDI usb_midi;

    // 2. それをラップする SerialMIDI<>
    MIDI_NAMESPACE::SerialMIDI<Adafruit_USBD_MIDI> serialMidiImpl;

    // 3. さらに高レベルな MidiInterface<>
    MIDI_NAMESPACE::MidiInterface<
        MIDI_NAMESPACE::SerialMIDI<Adafruit_USBD_MIDI>
    > midi;


};

#endif