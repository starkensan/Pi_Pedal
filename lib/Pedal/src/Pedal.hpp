#ifndef Pedal_h
#define Pedal_h

#include<Arduino.h>
#include <Adafruit_TinyUSB.h>
#include <DebugLog.h>

class Pedal{
	public:
		Pedal(const int pin, unsigned long debounceDelay = 50);

		void begin(bool isPullup = false);
		void attachCallback(void (*callback)(bool state));
		void update();
		bool getState();
		int getPin();

	private:
		const int pin;
		int currentState;
		int previousState;
		unsigned long lastChangeTime; // 最後に状態が変わった時間
		unsigned long debounceDelay;    // デバウンス時間

		void (*pedalCallback)(bool) = nullptr;
};

#endif