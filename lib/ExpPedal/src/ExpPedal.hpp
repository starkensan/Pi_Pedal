#ifndef ExpPedal_h
#define ExpPedal_h

#include <Arduino.h>
#include <DebugLog.h>
#include <Wire.h>
#include <Adafruit_MCP3421.h>

#include <HalExpPedal.hpp>

#define DEFAULT_ADC_ADDRESS 0x68

class ExpPedal : public HalExpPedal {
public:
    ExpPedal(int thresholdValue = 0);

    void begin(TwoWire* wireInstance, int sdaPin, int sclPin, uint8_t address = DEFAULT_ADC_ADDRESS) override;    // Initialize I2C and pins
    void update() override;   // Update both expression pedal and switches
    void attachCallback(void (*callback)(int)) override; // Attach callback for expression pedal changes
    int getValue() override; // Get current smoothed value

private:
    Adafruit_MCP3421 mcp;      // MCP3421 ADC instance
    uint8_t adcAddress;            // I2C address for the MCP3421

    int previousValue;           // Last sent value

    int threshold;                 // Hysteresis threshold for expression updates

    void (*expPedalCallback)(int); // Callback function pointer
};

#endif