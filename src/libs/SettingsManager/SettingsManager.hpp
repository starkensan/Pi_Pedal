#ifndef SETTINGCONTROLLER_SRC_SETTINGSMANAGER_HPP_
#define SETTINGCONTROLLER_SRC_SETTINGSMANAGER_HPP_
#include <stdint.h>
#include <string.h>
#include <HalStorage.hpp>
#include <config.h>

class SettingsManager {
public:
    // ==== ペダルの動作モード (何を送るか) ====
    enum class PedalMode : uint8_t {
        NONE = 0,     // 
        CC = 1,       // Send Control Change
        PC_NEXT = 2,  // Program Change: next preset
        PC_BACK = 3   // Program Change: previous preset
    };

    // ==== スイッチの押し方の意味 (どう送るか) ====
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

public:
    explicit SettingsManager(HalStorage& storage)
    : storage_(storage)
    , initialized_(false)
    , dirty_(false)
    {
        ::memset(&ramSettings_, 0, sizeof(ramSettings_));
    }

    /**
     * @brief 管理の初期化
     * - storage.begin() 呼び出し
     * - EEPROM→RAM読み込み
     * - 不正ならデフォルト生成＆保存
     */
    bool begin();

    // 全設定（RAM上キャッシュ）を取得
    const Settings& getAllSettings() const {
        return ramSettings_;
    }

    // 指定ペダルの設定取得
    const PedalSettings& getPedalSettings(size_t pedalIndex) const {
        return ramSettings_.pedal[pedalIndex];
    }

    // ===== Setter群 (RAMのみ更新、EEPROMはまだ) =====

    void setPedalMode(size_t pedalIndex, PedalMode mode);
    void setMidiChannel(size_t pedalIndex, uint8_t ch);
    void setCCNumber(size_t pedalIndex, uint8_t cc);
    void setSwitchBehavior(size_t pedalIndex, SwitchBehavior behavior);

    void setPedalSettings(size_t pedalIndex, const PedalSettings& ps);
    void setAllSettings(const Settings& s);

    /**
     * @brief RAM→EEPROMへ反映（dirty_のときだけ）
     */
    bool commitSettings();

    void loadFactoryDefaults();  // デフォルト設定をRAM+EEPROMへ

private:
    bool loadFromStorage();      // EEPROM -> RAM
    bool writeToStorage();       // RAM -> EEPROM
    static uint16_t calcCrc(const Settings& s);

private:
    HalStorage& storage_;
    bool initialized_;
    bool dirty_;
    Settings ramSettings_;

    static constexpr int   kStorageAddr         = 0;
    static constexpr size_t kRequiredStorageSize = sizeof(Settings);

    // <<<<<< ここ変更: versionを2に上げた >>>>>>
    static constexpr uint8_t kCurrentVersion    = 2;
};

#endif // SETTINGCONTROLLER_SRC_SETTINGSMANAGER_HPP_