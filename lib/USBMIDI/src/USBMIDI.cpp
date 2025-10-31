#include "USBMIDI.hpp"

USBMIDI::USBMIDI()
    : usb_midi()                           // デフォルト構築
    , serialMidiImpl(usb_midi)             // SerialMIDI(Adafruit_USBD_MIDI &)
    , midi(serialMidiImpl)                     // MidiInterface(SerialMIDI &)
{
}

void USBMIDI::begin( String device_name ){
    if (!TinyUSBDevice.isInitialized()) {
        TinyUSBDevice.begin(0);
    }
    usb_midi.setStringDescriptor(device_name.c_str());
    midi.begin(MIDI_CHANNEL_OMNI);

    while( !TinyUSBDevice.mounted() ) delay(1);

    if (TinyUSBDevice.mounted()) {
        TinyUSBDevice.detach();
        delay(10);
        TinyUSBDevice.attach();
    }
    LOG_INFO("[USBMIDI] USB Midi init");
}

void USBMIDI::sendControlChange(uint8_t channel, uint8_t control, uint8_t value){
    midi.sendControlChange( control, value, channel );
    LOG_INFO("[USBMIDI] Send CC, CCNum: ", control, ", Value: ", value, ", Channel: ",  channel);
}

void USBMIDI::sendProgramChange(uint8_t channel, uint8_t program){
    midi.sendProgramChange( program, channel );
    LOG_INFO("[USBMIDI] Send CC, PCNum: ", program, ", Channel: ", channel);
}