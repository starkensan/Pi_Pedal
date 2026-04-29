#ifndef SETTINGSMANAGER_HPP_
#define SETTINGSMANAGER_HPP_
#include <stdint.h>
#include <string.h>
#include <HalStorage.hpp>
#include <config.h>
#include "SettingsDefs.hpp"
#include "SettingsLock.hpp"
#include "SettingsStore.hpp"

using namespace SettingsDefs;

class SettingsManager : public SettingsStore {
public:
    explicit SettingsManager(HalStorage& storage)
    : storage_(storage)
    , initialized_(false)
    , dirty_(false)
    {
        lock_.init();
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
    const Settings& getAllSettings() const override;

    /**
     * @brief 指定ペダルの設定取得
     * @param pedalIndex ペダルインデックス
     * @return ペダル設定(PedalSettings)
    */
    const PedalSettings& getPedalSettings(size_t pedalIndex) const override;

    /**
     * @brief Setter群 (RAMのみ更新、EEPROMはまだ)
     * @param pedalIndex ペダルインデックス
     * @param mode ペダルモード(PedalMode::CC, PC_NEXT, PC_BACK)
     */
    void setPedalMode(size_t pedalIndex, PedalMode mode) override;
    
    /**
     * @brief MIDIチャンネル設定
     * @param pedalIndex ペダルインデックス
     * @param ch MIDIチャンネル(1-16)
     */
    void setMidiChannel(size_t pedalIndex, uint8_t ch) override;

    /**
     * @brief CC番号設定
     * @param pedalIndex ペダルインデックス
     * @param cc CC番号(0-127)
     */
    void setCCNumber(size_t pedalIndex, uint8_t cc) override;

    /**
     * @brief スイッチ動作設定
     * @param pedalIndex ペダルインデックス
     * @param behavior スイッチ動作(SwitchBehavior::MOMENTARY/TOGGLE)
     */
    void setSwitchBehavior(size_t pedalIndex, SwitchBehavior behavior) override;


    /**
     * @brief ペダル設定一括設定
     * @param pedalIndex ペダルインデックス
     * @param ps ペダル設定(PedalSettings)
     */
    void setPedalSettings(size_t pedalIndex, const PedalSettings& ps) override;

    /**
     * @brief 全設定一括設定
     * @param s 全設定(Settings)
     */
    void setAllSettings(const Settings& s) override;

    /**
     * @brief RAM→EEPROMへ反映（dirty_のときだけ）
     */
    bool commitSettings() override;

    /**
     * @brief dirty_を強制的にfalseにする（EEPROM反映済み扱いにする）
     */
    void uncommitSettings() override;

    /**
     * @brief デフォルト設定をRAM+EEPROMへ反映
     */
    void FactoryReset() override;

    /**
     * @brief 設定がEEPROMに反映されていないかどうか取得
     * @return dirty_の値
     */
    bool getIsDirty() const override {
        LockGuard lock(lock_);
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
    mutable SettingsLock lock_;
    struct LockGuard {
        SettingsLock& lock;
        LockGuard(SettingsLock& lock_) : lock(lock_) { lock.lock(); }
        ~LockGuard() { lock.unlock(); }
    };

};

#endif // SETTINGSMANAGER_HPP_
