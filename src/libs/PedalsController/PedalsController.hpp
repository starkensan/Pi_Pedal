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
    PedalsController(HalPedal& pedals,
                     HalExpPedal& expPedal,
                     HalUSBMIDI& usbMIDI,
                     SettingsManager& settings)
    : pedals_(pedals)
    , expPedal_(expPedal)
    , usbMIDI_(usbMIDI)
    , settings_(settings)
    {
        self = this;
    }

    /**
     * @brief 初期化処理
     * @param DeviceName USB MIDIデバイス名
    */
    void begin(String DeviceName);

    /**
     * @brief MIDI送信開始
     */
    void start();

    /**
     * @brief MIDI送信停止
     */
    void stop();

    /**
     * @brief 定期処理
     */
    void update();
private:
    HalPedal&    pedals_;
    HalExpPedal& expPedal_;
    HalUSBMIDI&  usbMIDI_;
    SettingsManager& settings_;

    static PedalsController* self;

    const int pedalPins_[MAX_PEDALS-1] = {PEDAL1_PIN, PEDAL2_PIN, PEDAL3_PIN, PEDAL4_PIN, PEDAL5_PIN, PEDAL6_PIN};

    bool CCPedalState_[MAX_PEDALS-1];
    uint8_t PCCurrentNumber_ = 0;
    
    bool midiStarted_ = false;

    static void pedalsCallbackStatic(int index, bool state);
    static void expPedalsCallbackStatic(int value);

    void pedalsCallback(int index, bool state);
    void expPedalCallback(int value);
};

#endif // PEDALS_CONTROLLER_HPP