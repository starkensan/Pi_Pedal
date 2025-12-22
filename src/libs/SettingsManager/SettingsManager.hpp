#ifndef SETTINGSMANAGER_HPP_
#define SETTINGSMANAGER_HPP_
#include <stdint.h>
#include <string.h>
#include <HalStorage.hpp>
#include <config.h>
#include "SettingsDefs.hpp"

using namespace SettingsDefs;

class SettingsManager {
public:
    explicit SettingsManager(HalStorage& storage)
    : storage_(storage)
    , initialized_(false)
    , dirty_(false)
    {
        mutex_init(&mtx_);
    }

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
    const Settings& getAllSettings() const;

    /**
     * @brief 指定ペダルの設定取得
     * @param pedalIndex ペダルインデックス
     * @return ペダル設定(PedalSettings)
    */
    const PedalSettings& getPedalSettings(size_t pedalIndex) const;

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
     * @brief dirty_を強制的にfalseにする（EEPROM反映済み扱いにする）
     */
    void uncommitSettings();

    /**
     * @brief デフォルト設定をRAM+EEPROMへ反映
     */
    void FactoryReset();

    /**
     * @brief 設定がEEPROMに反映されていないかどうか取得
     * @return dirty_の値
     */
    bool getIsDirty() const {
        LockGuard lock(mtx_);
        return dirty_;
    }

private:
    SettingsManager() = default;
    SettingsManager(const SettingsManager&) = delete;
    SettingsManager& operator=(const SettingsManager&) = delete;

    bool loadFromStorage();      // EEPROM -> RAM
    bool writeToStorage();       // RAM -> EEPROM
    void loadFactoryDefaults();  // デフォルト設定生成

    HalStorage& storage_;
    Settings ramSettings_;

    static constexpr int   kStorageAddr         = 0;
    static constexpr size_t kRequiredStorageSize = sizeof(Settings);
    static constexpr uint8_t kCurrentVersion    = 1;

    bool initialized_;
    bool dirty_;
    mutable mutex_t mtx_;
    struct LockGuard {
        mutex_t& m;
        LockGuard(mutex_t& m_) : m(m_) { mutex_enter_blocking(&m); }
        ~LockGuard() { mutex_exit(&m); }
    };

};

#endif // SETTINGSMANAGER_HPP_