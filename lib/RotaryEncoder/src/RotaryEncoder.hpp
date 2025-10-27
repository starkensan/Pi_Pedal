#ifndef ROTARY_ENCODER_HPP
#define ROTARY_ENCODER_HPP

#include <Arduino.h>
#include <HalRotaryEncoder.hpp>

class RotaryEncoder : public HalRotaryEncoder {
public:
    RotaryEncoder(int DT, int CLK, int SW, int RotaryDebounceTime = 1000, int SwitchDebounceTime = 100);

    void attachRotaryCallback(void (*rotaryCallback)(int direction, int count)) override;
    void attachSwitchCallback(void (*swCallback)()) override;
    void checkEncoder() override;
    void checkSwitch() override;
    int getCount() override;
    int getDirection() override;
    void setCount(int count) override;
    void clearCount() override;

private:
    void (*_rotaryCallback)(int, int);
    void (*_swCallback)();
    int _count = 0;
    int _direction = 0;

    int _DTPIN;
    int _CLKPIN;
    int _SWPIN;
    int _ROTARYDEBOUNCE;
    int _SWITCHDEBOUNCE;

    unsigned long _rotaryChangeTime;
};

#endif // ROTARY_ENCODER_HPP
