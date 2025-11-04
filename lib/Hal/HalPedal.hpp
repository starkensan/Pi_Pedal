#ifndef HALPEDAL_HPP
#define HALPEDAL_HPP

#include <Arduino.h>
#include <config.h>

class HalPedal {
public:
    virtual ~HalPedal() {}

    /**
     * @brief 初期化処理
     * @param isPullup プルアップ入力を使うかどうか
     */
    virtual void begin(const int (&pins)[MAX_PEDALS-1],bool isPullup = true) = 0;

    /**
     * @brief 状態変化時に呼ばれるコールバックを登録
     * @param callback スイッチ状態(true=押下)を引数に取る関数ポインタ
     */
    virtual void attachCallback(void (*callback)(int index, bool state)) = 0;

    /**
     * @brief 入力状態を更新し、変化があればコールバックを呼ぶ
     */
    virtual void update() = 0;

    /**
     * @brief 現在のスイッチ状態を取得
     * @param PedalNum ペダル番号
     * @return true: ON / false: OFF
     */
    virtual bool getState(int index) = 0;

    /**
     * @brief 接続ピン番号を取得\
     * @param PedalNum ペダル番号
     * @return ピン番号
     */
    virtual int getPin(int index) = 0;
};

#endif
