#ifndef PEDALS_CONTROLLER_HPP
#define PEDALS_CONTROLLER_HPP
#include <stdint.h>
#include <DebugLog.h>
#include <HalPedal.hpp>
#include <HalExpPedal.hpp>
#include <HalUSBMIDI.hpp>
#include <HalStorage.hpp>
#include <SettingsManager/SettingsManager.hpp>
#include <config.h>

class PedalsController {
public:
    PedalsController(HalPedal* pedals,
                     HalExpPedal& expPedal,
                     HalUSBMIDI& usbMIDI,
                     HalStorage& storage)
    : pedals_(pedals)
    , expPedal_(expPedal)
    , usbMIDI_(usbMIDI)
    , storage_(storage)
    {
        SettingsManager settings(storage_);
        settings_ = &settings;
        self = this;
    }

    // 初期化
    void begin(String DeviceName);

    void start();

    void stop();

    // 定期処理
    void update();
private:
    HalPedal*    pedals_;
    HalExpPedal& expPedal_;
    HalUSBMIDI&  usbMIDI_;
    HalStorage& storage_;
    SettingsManager *settings_ = nullptr;

    static PedalsController* self;

    const int pedalPins_[MAX_PEDALS-1] = {PEDAL1_PIN, PEDAL2_PIN, PEDAL3_PIN, PEDAL4_PIN, PEDAL5_PIN, PEDAL6_PIN};

    bool CCPedalState_[MAX_PEDALS-1];


    static void pedalsCallbackStatic(int index, bool state);
    static void expPedalsCallbackStatic(int value);

    void pedalsCallback(int index, bool state);
    void expPedalCallback(int value);
};

#endif // PEDALS_CONTROLLER_HPP