#include "SettingsManager.hpp"

struct LockGuard {
  mutex_t& m;
  LockGuard(mutex_t& m_) : m(m_) { mutex_enter_blocking(&m); }
  ~LockGuard() { mutex_exit(&m); }
};


// EEPROM -> RAM
bool SettingsManager::loadFromStorage() {
    Settings tmp;
    storage_.get(kStorageAddr, &tmp, sizeof(Settings));

    // バージョン不一致なら使用不可扱い
    if (tmp.version != kCurrentVersion) {
        return false;
    }

    // OKなのでRAMに採用
    ramSettings_ = tmp;
    return true;
}

// RAM -> EEPROM
bool SettingsManager::writeToStorage() {

    // EEPROMに書き込み
    storage_.put(kStorageAddr, &ramSettings_, sizeof(Settings));

    // 物理反映
    if (!storage_.commit()) {
        return false;
    }
    return true;
}

// デフォルト設定を作成し、EEPROMにも保存する
void SettingsManager::loadFactoryDefaults() {
    ramSettings_.version = kCurrentVersion;

    for (size_t i = 0; i < MAX_PEDALS; ++i) {
        ramSettings_.pedal[i].pedalMode      = PedalMode::CC;
        ramSettings_.pedal[i].midiChannel    = 1;   // MIDI Ch1
        ramSettings_.pedal[i].ccNumber       = 1+i;  // 例: expression的CC
        ramSettings_.pedal[i].switchBehavior = SwitchBehavior::MOMENTARY; // デフォ:ラッチ動作
        ramSettings_.pedal[i]._pad           = 0;
    }

    storage_.put(kStorageAddr, &ramSettings_, sizeof(Settings));
    storage_.commit();

    dirty_ = false;
}

/*
@brief 管理の初期化
*/
bool SettingsManager::begin() {
    if (initialized_) {
        return true;
    }

    // 必要サイズを確保
    storage_.begin(kRequiredStorageSize);

    // 読み込みトライ
    if (!loadFromStorage()) {
        // 未初期化 or 壊れてるのでデフォルト使う
        loadFactoryDefaults();
    }

    initialized_ = true;
    dirty_ = false; // いまはRAMとEEPROMが同期している
    return true;
}


/*
@brief Setter群 (RAMのみ更新、EEPROMはまだ)
@param pedalIndex ペダルインデックス
@param mode ペダルモード(PedalMode::CC/PC_NEXT/PC_BACK)
*/
void SettingsManager::setPedalMode(size_t pedalIndex, PedalMode mode) {
    if (pedalIndex >= MAX_PEDALS) return;

    if (ramSettings_.pedal[pedalIndex].pedalMode != mode) {
        ramSettings_.pedal[pedalIndex].pedalMode = mode;
        dirty_ = true;
    }
}

/*
@brief MIDIチャンネル設定
@param pedalIndex ペダルインデックス
@param ch MIDIチャンネル(1-16)
*/
void SettingsManager::setMidiChannel(size_t pedalIndex, uint8_t ch) {
    if (pedalIndex >= MAX_PEDALS) return;
    // 必要なら1〜16にクリップとかする
    if (ramSettings_.pedal[pedalIndex].midiChannel != ch) {
        ramSettings_.pedal[pedalIndex].midiChannel = ch;
        dirty_ = true;
    }
}

/*
@brief CC番号設定
@param pedalIndex ペダルインデックス
@param cc CC番号(0-127)
*/
void SettingsManager::setCCNumber(size_t pedalIndex, uint8_t cc) {
    if (pedalIndex >= MAX_PEDALS) return;

    if (ramSettings_.pedal[pedalIndex].ccNumber != cc) {
        ramSettings_.pedal[pedalIndex].ccNumber = cc;
        dirty_ = true;
    }
}

/*
@brief スイッチ動作設定
@param pedalIndex ペダルインデックス
@param behavior スイッチ動作(SwitchBehavior::MOMENTARY/TOGGLE)
*/
void SettingsManager::setSwitchBehavior(size_t pedalIndex, SwitchBehavior behavior) {
    if (pedalIndex >= MAX_PEDALS) return;

    if (ramSettings_.pedal[pedalIndex].switchBehavior != behavior) {
        ramSettings_.pedal[pedalIndex].switchBehavior = behavior;
        dirty_ = true;
    }
}

/*
@brief ペダル設定一括設定
@param pedalIndex ペダルインデックス
@param ps ペダル設定(PedalSettings)
*/
void SettingsManager::setPedalSettings(size_t pedalIndex, const PedalSettings& ps) {
    if (pedalIndex >= MAX_PEDALS) return;

    bool changed = false;

    if (ramSettings_.pedal[pedalIndex].pedalMode != ps.pedalMode) {
        ramSettings_.pedal[pedalIndex].pedalMode = ps.pedalMode;
        changed = true;
    }
    if (ramSettings_.pedal[pedalIndex].midiChannel != ps.midiChannel) {
        ramSettings_.pedal[pedalIndex].midiChannel = ps.midiChannel;
        changed = true;
    }
    if (ramSettings_.pedal[pedalIndex].ccNumber != ps.ccNumber) {
        ramSettings_.pedal[pedalIndex].ccNumber = ps.ccNumber;
        changed = true;
    }
    if (ramSettings_.pedal[pedalIndex].switchBehavior != ps.switchBehavior) {
        ramSettings_.pedal[pedalIndex].switchBehavior = ps.switchBehavior;
        changed = true;
    }

    if (changed) {
        dirty_ = true;
    }
}

/*
@brief 全設定一括設定
@param s 全設定(Settings)
*/
void SettingsManager::setAllSettings(const Settings& s) {
    bool changed = false;

    for (size_t i = 0; i < MAX_PEDALS; ++i) {
        if (ramSettings_.pedal[i].pedalMode != s.pedal[i].pedalMode) {
            ramSettings_.pedal[i].pedalMode = s.pedal[i].pedalMode;
            changed = true;
        }
        if (ramSettings_.pedal[i].midiChannel != s.pedal[i].midiChannel) {
            ramSettings_.pedal[i].midiChannel = s.pedal[i].midiChannel;
            changed = true;
        }
        if (ramSettings_.pedal[i].ccNumber != s.pedal[i].ccNumber) {
            ramSettings_.pedal[i].ccNumber = s.pedal[i].ccNumber;
            changed = true;
        }
        if (ramSettings_.pedal[i].switchBehavior != s.pedal[i].switchBehavior) {
            ramSettings_.pedal[i].switchBehavior = s.pedal[i].switchBehavior;
            changed = true;
        }
    }

    // バージョンは現在のものを維持
    ramSettings_.version = kCurrentVersion;

    if (changed) {
        dirty_ = true;
    }
}

// -------------------------------------------------
// commitSettings()
// RAM→EEPROM反映
bool SettingsManager::commitSettings() {
    if (!dirty_) {
        return true; // 保存する変更なし
    }

    bool ok = writeToStorage();
    if (ok) {
        dirty_ = false;
    }
    return ok;
}
