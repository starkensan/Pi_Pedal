#ifndef SETTINGSDEFS_H
#define SETTINGSDEFS_H
#include <stdint.h>
#include <config.h>

namespace SettingsDefs{
    // ==== ペダルの動作モード ====
    enum class PedalMode : uint8_t {
        NONE = 0,     // 
        CC = 1,       // Send Control Change
        PC_NEXT = 2,  // Program Change: next preset
        PC_BACK = 3   // Program Change: previous preset
    };

    // ==== スイッチの判定モード ====
    enum class SwitchBehavior : uint8_t {
        MOMENTARY = 0, // 押してる間だけON
        TOGGLE    = 1  // 押すたびにON/OFFが反転
    };

    // ==== 1つのペダルが持つ設定 ====
    struct PedalSettings {
        PedalMode pedalMode;        // 何を送るか (CC / PC_NEXT / PC_BACK)
        uint8_t   midiChannel;      // 1-16
        uint8_t   ccNumber;         // CC番号
        SwitchBehavior switchBehavior; // モーメンタリ or トグル
        uint8_t   _pad;             // アラインメント/将来拡張用
    };

    // ==== 全体設定 ====
    struct Settings {
        uint8_t version;                        // 互換性管理
        PedalSettings pedal[MAX_PEDALS];        // ペダルごとの設定
    };
}

#endif // SETTINGSDEFS_H