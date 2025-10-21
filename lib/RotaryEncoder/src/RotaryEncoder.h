#ifndef RotaryEncoder_h
#define RotaryEncoder_h

#include<Arduino.h>

class RotaryEncoder{
	public:
		RotaryEncoder(int DT, int CLK, int SW, int RotaryDebounceTime=1000, int SwitchDebounceTime=100);
		void attachRotaryCallback(void (*rotaryCallback)(int direction, int count));
		void attachSwitchCallback(void (*swCallback)());
		void checkEncoder();
		void checkSwitch();
		int getCount();
		int getDirection();
		void setCount(int count);
		void clearCount();

	private:
		void (*_rotaryCallback)(int, int);
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