#ifndef RotaryEncoder_h
#define RotaryEncoder_h

#include<Arduino.h>

class RotaryEncoder{
  public:
    RotaryEncoder(int DT, int CLK, int SW, int RotaryDebounceTime, int SwitchDebounceTime);
    void attachRotaryCallback(void (*rotaryCallback)());
    void attachSwitchCallback(void (*swCallback)());
    void checkEncoder();
    void checkSwitch();
    int getCount();
    int getDirection();
    void setCount(int count);
    void clearCount();

  private:
    void (*_rotaryCallback)();
    void (*_swCallback)();
    int _count=0;
    int _direction=0;

    int _DTPIN;
    int _CLKPIN;
    int _SWPIN;
    int _ROTARYDEBOUNCE;
    int _SWITCHDEBOUNCE;

    unsigned long _rotaryChangeTime;

};

#endif