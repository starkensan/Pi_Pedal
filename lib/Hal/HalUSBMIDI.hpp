#ifndef HALUSBMIDI_HPP
#define HALUSBMIDI_HPP

#include <Arduino.h>

class HalUSBMIDI {
public:
    virtual ~HalUSBMIDI() {}

    /**
     * @brief デバイスとして初期化する
     * @param device_name ホストPC等に見せるデバイス名
     */
    virtual void begin(String device_name = "PiPedal") = 0;

    /**
     * @brief Control Change (CC) を送信
     * @param channel MIDIチャンネル (1-16想定)
     * @param control CC番号
     * @param value   値(0-127)
     */
    virtual void sendControlChange(uint8_t channel,
                                   uint8_t control,
                                   uint8_t value) = 0;

    /**
     * @brief Program Change (PC) を送信
     * @param channel MIDIチャンネル (1-16想定)
     * @param program プログラム番号(0-127など)
     */
    virtual void sendProgramChange(uint8_t channel,
                                   uint8_t program) = 0;
};

#endif // IUSBMIDI_HPP
