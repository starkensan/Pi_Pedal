#ifndef HALPEDAL_HPP
#define HALPEDAL_HPP

#include <Arduino.h>

class HalPedal {
public:
    virtual ~HalPedal() {}

    /**
     * @brief 初期化処理
     * @param isPullup プルアップ入力を使うかどうか
     */
    virtual void begin(bool isPullup = false) = 0;

    /**
     * @brief 状態変化時に呼ばれるコールバックを登録
     * @param callback スイッチ状態(true=押下)を引数に取る関数ポインタ
     */
    virtual void attachCallback(void (*callback)(bool state)) = 0;

    /**
     * @brief 入力状態を更新し、変化があればコールバックを呼ぶ
     */
    virtual void update() = 0;

    /**
     * @brief 現在のスイッチ状態を取得
     * @return true: ON / false: OFF
     */
    virtual bool getState() = 0;

    /**
     * @brief 接続ピン番号を取得
     * @return ピン番号
     */
    virtual int getPin() = 0;
};

#endif
