#ifndef PedalMonitor_h
#define PedalMonitor_h

#include <Adafruit_TinyUSB.h>
#include <MIDI.h>

#include<Arduino.h>

class PedalMonitor{
  public:
    PedalMonitor(midi::MidiInterface<midi::SerialMIDI<Adafruit_USBD_MIDI>>* midiInstance, const int* pins, int* PedalControlNumbers, int SwitchesNumber, unsigned long debounceDelay = 50);
    ~PedalMonitor();

    void begin();
    void update();

  private:
    midi::MidiInterface<midi::SerialMIDI<Adafruit_USBD_MIDI>>* midi;
    const int* pins;
    int* controlNumbers;
    int numSwitches;
    int* currentStates;
    int* previousStates;
    unsigned long* lastChangeTimes; // 最後に状態が変わった時間
    unsigned long debounceDelay;    // デバウンス時間

    void sendSwitchesMIDI(int switchIndex, int state);

};

#endif