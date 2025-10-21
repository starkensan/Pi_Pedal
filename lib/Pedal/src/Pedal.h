#ifndef Pedal_h
#define Pedal_h

#include<Arduino.h>
#include <Adafruit_TinyUSB.h>
#include <MIDI.h>

class Pedal{
  public:
    Pedal(midi::MidiInterface<midi::SerialMIDI<Adafruit_USBD_MIDI>>* midiInstance, const int* pins, const int* PedalControlNumbers, int SwitchesNumber, unsigned long debounceDelay = 50);
    ~Pedal();

    void begin();
    void update();

  private:
    midi::MidiInterface<midi::SerialMIDI<Adafruit_USBD_MIDI>>* midi;
    const int* pins;
    const int* controlNumbers;
    int numSwitches;
    int* currentStates;
    int* previousStates;
    unsigned long* lastChangeTimes; // 最後に状態が変わった時間
    unsigned long debounceDelay;    // デバウンス時間

    void sendSwitchesMIDI(int switchIndex, int state);

};

#endif