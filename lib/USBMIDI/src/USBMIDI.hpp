#ifndef USBMIDI_HPP
#define USBMIDI_HPP

#include <Arduino.h>
#include <Adafruit_TinyUSB.h>
#include <MIDI.h>
#include "HalUSBMIDI.hpp"

class USBMIDI : public HalUSBMIDI {
public:
    USBMIDI();

    void begin(String device_name = "PiPedal") override;

    void sendControlChange(uint8_t channel,
                           uint8_t control,
                           uint8_t value) override;

    void sendProgramChange(uint8_t channel,
                           uint8_t program) override;

private:
    // TinyUSB MIDI デバイス
    Adafruit_USBD_MIDI usb_midi;

    // それをラップする SerialMIDI
    MIDI_NAMESPACE::SerialMIDI<Adafruit_USBD_MIDI> serialMidiImpl {
        usb_midi
    };

    // さらに高レベルな MidiInterface
    MIDI_NAMESPACE::MidiInterface<
        MIDI_NAMESPACE::SerialMIDI<Adafruit_USBD_MIDI>
    > midi { serialMidiImpl };
};

#endif // USBMIDI_HPP
