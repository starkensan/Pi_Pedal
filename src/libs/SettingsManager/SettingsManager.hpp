#ifndef SETTINGSMANAGER_HPP_
#define SETTINGSMANAGER_HPP_
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
    {}

    /**
     * @brief 管理の初期化
     * - storage.begin() 呼び出し
     * - EEPROM→RAM読み込み
     * - 不正ならデフォルト生成＆保存
     */
    bool begin();

    /**
     * @brief 全設定取得
     * @return 全設定(Settings)
    */
    const Settings& getAllSettings() const {
        return ramSettings_;
    }

    /**
     * @brief 指定ペダルの設定取得
     * @param pedalIndex ペダルインデックス
     * @return ペダル設定(PedalSettings)
    */
    const PedalSettings& getPedalSettings(size_t pedalIndex) const {
        return ramSettings_.pedal[pedalIndex];
    }

    /**
     * @brief Setter群 (RAMのみ更新、EEPROMはまだ)
     * @param pedalIndex ペダルインデックス
     * @param mode ペダルモード(PedalMode::CC, PC_NEXT, PC_BACK)
     */
    void setPedalMode(size_t pedalIndex, PedalMode mode);
    
    /**
     * @brief MIDIチャンネル設定
     * @param pedalIndex ペダルインデックス
     * @param ch MIDIチャンネル(1-16)
     */
    void setMidiChannel(size_t pedalIndex, uint8_t ch);

    /**
     * @brief CC番号設定
     * @param pedalIndex ペダルインデックス
     * @param cc CC番号(0-127)
     */
    void setCCNumber(size_t pedalIndex, uint8_t cc);

    /**
     * @brief スイッチ動作設定
     * @param pedalIndex ペダルインデックス
     * @param behavior スイッチ動作(SwitchBehavior::MOMENTARY/TOGGLE)
     */
    void setSwitchBehavior(size_t pedalIndex, SwitchBehavior behavior);


    /**
     * @brief ペダル設定一括設定
     * @param pedalIndex ペダルインデックス
     * @param ps ペダル設定(PedalSettings)
     */
    void setPedalSettings(size_t pedalIndex, const PedalSettings& ps);

    /**
     * @brief 全設定一括設定
     * @param s 全設定(Settings)
     */
    void setAllSettings(const Settings& s);

    /**
     * @brief RAM→EEPROMへ反映（dirty_のときだけ）
     */
    bool commitSettings();

    /**
     * @brief デフォルト設定をRAM+EEPROMへ反映
     */
    void loadFactoryDefaults();  

private:
    SettingsManager() = default;
    SettingsManager(const SettingsManager&) = delete;
    SettingsManager& operator=(const SettingsManager&) = delete;

    bool loadFromStorage();      // EEPROM -> RAM
    bool writeToStorage();       // RAM -> EEPROM

    HalStorage& storage_;
    bool initialized_;
    bool dirty_;
    Settings ramSettings_;

    static constexpr int   kStorageAddr         = 0;
    static constexpr size_t kRequiredStorageSize = sizeof(Settings);

    static constexpr uint8_t kCurrentVersion    = 1;
};

#endif // SETTINGSMANAGER_HPP_