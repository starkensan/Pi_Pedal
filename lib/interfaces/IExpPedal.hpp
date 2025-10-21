#ifndef IEXPPEDAL_HPP
#define IEXPPEDAL_HPP

#include <Arduino.h>
#include <Wire.h>

class IExpPedal {
public:
    virtual ~IExpPedal() {}

    /**
     * @brief 初期化処理
     * @param wireInstance 使用するI2Cインスタンス（例：&Wire）
     * @param sdaPin SDAピン番号
     * @param sclPin SCLピン番号
     * @param address ADCのI2Cアドレス（デフォルト 0x68）
     */
    virtual void begin(TwoWire* wireInstance,
                       int sdaPin,
                       int sclPin,
                       uint8_t address = 0x68) = 0;

    /**
     * @brief ペダル状態を更新（ADC値を読み取る）
     */
    virtual void update() = 0;

    /**
     * @brief コールバック関数を登録
     * @param callback int型の値を引数に取る関数ポインタ
     */
    virtual void attachCallback(void (*callback)(int)) = 0;

    /**
     * @brief 現在のスムージング済みのペダル値を取得
     * @return 0〜4095などのADC値（範囲は実装依存）
     */
    virtual int getValue() = 0;
};

#endif // IEXPPEDAL_HPP
