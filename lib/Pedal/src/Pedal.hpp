#ifndef PEDAL_HPP
#define PEDAL_HPP

#include <Arduino.h>
#include <DebugLog.h>
#include <HalPedal.hpp>
#include <config.h>

class Pedal : public HalPedal {
public:
    Pedal(unsigned long debounceDelay = 50);

    void begin(const int (&pins)[MAX_PEDALS-1], bool isPullup = false) override;
    void attachCallback(void (*callback)(int PedalNum, bool state)) override;
    void update() override;
    bool getState(int PedalNum) override;
    int getPin(int PedalNum) override;

private:
    int (pins_)[MAX_PEDALS-1];
    int currentState[MAX_PEDALS-1];
    int previousState[MAX_PEDALS-1];
    unsigned long lastChangeTime[MAX_PEDALS-1]; // 最後に状態が変わった時間
    unsigned long debounceDelay;  // デバウンス時間

    void (*pedalCallback)(int, bool) = nullptr;
};

#endif // PEDAL_HPP
