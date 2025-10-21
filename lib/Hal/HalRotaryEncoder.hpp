#ifndef HAL_ROTARY_ENCODER_HPP
#define HAL_ROTARY_ENCODER_HPP

#include <Arduino.h>

class HalRotaryEncoder {
public:
    virtual ~HalRotaryEncoder() {}

    /**
     * @brief エンコーダ回転時のコールバックを登録
     * @param rotaryCallback 回転方向とカウントを受け取る関数ポインタ
     */
    virtual void attachRotaryCallback(void (*rotaryCallback)(int direction, int count)) = 0;

    /**
     * @brief スイッチ押下時のコールバックを登録
     * @param swCallback 押下イベント時に呼ばれる関数ポインタ
     */
    virtual void attachSwitchCallback(void (*swCallback)()) = 0;

    /**
     * @brief 回転をチェックし、必要ならコールバックを呼ぶ
     */
    virtual void checkEncoder() = 0;

    /**
     * @brief スイッチ押下をチェックし、必要ならコールバックを呼ぶ
     */
    virtual void checkSwitch() = 0;

    /**
     * @brief 現在のカウント値を取得
     * @return カウント値
     */
    virtual int getCount() = 0;

    /**
     * @brief 回転方向を取得
     * @return -1: 左 / 1: 右 / 0: 変化なし
     */
    virtual int getDirection() = 0;

    /**
     * @brief カウント値を設定
     * @param count 設定するカウント値
     */
    virtual void setCount(int count) = 0;

    /**
     * @brief カウント値をリセット
     */
    virtual void clearCount() = 0;
};

#endif // IROTARY_ENCODER_HPP
