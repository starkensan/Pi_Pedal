#include "SettingsManager.hpp"

// local RAII
struct LockGuard {
  mutex_t& m;
  LockGuard(mutex_t& m_) : m(m_) { mutex_enter_blocking(&m); }
  ~LockGuard() { mutex_exit(&m); }
};

// -------------------------------------------------
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

// -------------------------------------------------
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

// -------------------------------------------------
// デフォルト設定を作成し、EEPROMにも保存する
void SettingsManager::loadFactoryDefaults() {
    ::memset(&ramSettings_, 0, sizeof(ramSettings_));
    ramSettings_.version = kCurrentVersion;

    for (size_t i = 0; i < MAX_PEDALS; ++i) {
        ramSettings_.pedal[i].pedalMode      = PedalMode::CC;
        ramSettings_.pedal[i].midiChannel    = 1;   // MIDI Ch1
        ramSettings_.pedal[i].ccNumber       = i+1;  // 例: expression的CC
        ramSettings_.pedal[i].switchBehavior = SwitchBehavior::MOMENTARY; // デフォ:モーメンタリ動作
        ramSettings_.pedal[i]._pad           = 0;
    }

    storage_.put(kStorageAddr, &ramSettings_, sizeof(Settings));
    storage_.commit();

    dirty_ = false;
}

// -------------------------------------------------
// begin()
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

// -------------------------------------------------
// Setter群 (RAMだけ更新してdirty_=true)

void SettingsManager::setPedalMode(size_t pedalIndex, PedalMode mode) {
    if (pedalIndex >= MAX_PEDALS) return;

    if (ramSettings_.pedal[pedalIndex].pedalMode != mode) {
        ramSettings_.pedal[pedalIndex].pedalMode = mode;
        dirty_ = true;
    }
}

void SettingsManager::setMidiChannel(size_t pedalIndex, uint8_t ch) {
    if (pedalIndex >= MAX_PEDALS) return;
    // 必要なら1〜16にクリップとかする
    if (ramSettings_.pedal[pedalIndex].midiChannel != ch) {
        ramSettings_.pedal[pedalIndex].midiChannel = ch;
        dirty_ = true;
    }
}

void SettingsManager::setCCNumber(size_t pedalIndex, uint8_t cc) {
    if (pedalIndex >= MAX_PEDALS) return;

    if (ramSettings_.pedal[pedalIndex].ccNumber != cc) {
        ramSettings_.pedal[pedalIndex].ccNumber = cc;
        dirty_ = true;
    }
}

void SettingsManager::setSwitchBehavior(size_t pedalIndex, SwitchBehavior behavior) {
    if (pedalIndex >= MAX_PEDALS) return;

    if (ramSettings_.pedal[pedalIndex].switchBehavior != behavior) {
        ramSettings_.pedal[pedalIndex].switchBehavior = behavior;
        dirty_ = true;
    }
}

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
