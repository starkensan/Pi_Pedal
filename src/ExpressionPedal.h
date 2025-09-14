#ifndef ExpressionPedal_h
#define ExpressionPedal_h

#include <Arduino.h>
#include <Adafruit_TinyUSB.h>
#include <Adafruit_MCP3421.h>
#include <MIDI.h>

#include<Arduino.h>


class ExpressionPedal {
private:
    midi::MidiInterface<midi::SerialMIDI<Adafruit_USBD_MIDI>>* midi;
    Adafruit_MCP3421 mcp;      // MCP3421 ADC instance
    uint8_t adcAddress;            // I2C address for the MCP3421
    int midiCCExpression;          // MIDI CC number for expression pedal

    float smoothedValue;           // Smoothed value for expression pedal
    int previousCCValue;           // Last sent CC value

    int threshold;                 // Hysteresis threshold for expression updates

    void sendExpressionMIDI(int ccValue);

public:
    ExpressionPedal(midi::MidiInterface<midi::SerialMIDI<Adafruit_USBD_MIDI>>* midiInstance, int midiCCExp, int thresholdValue = 0);

    void begin(TwoWire* wireInstance, int sdaPin = -1, int sclPin = -1, uint8_t address = 0x68);    // Initialize I2C and pins
    void update();   // Update both expression pedal and switches
    void ChagneControlNumber(int ControlNumber);
};

#endif