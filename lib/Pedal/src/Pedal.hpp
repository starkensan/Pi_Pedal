#ifndef PEDAL_HPP
#define PEDAL_HPP

#include <Arduino.h>
#include <DebugLog.h>
#include <HalPedal.hpp>

class Pedal : public HalPedal {
public:
    Pedal(const int pin, unsigned long debounceDelay = 50);

    void begin(bool isPullup = false) override;
    void attachCallback(void (*callback)(bool state)) override;
    void update() override;
    bool getState() override;
    int getPin() override;

private:
    const int pin;
    int currentState;
    int previousState;
    unsigned long lastChangeTime; // 最後に状態が変わった時間
    unsigned long debounceDelay;  // デバウンス時間

    void (*pedalCallback)(bool) = nullptr;
};

#endif // PEDAL_HPP
