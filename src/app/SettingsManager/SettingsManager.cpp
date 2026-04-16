#include "SettingsManager.hpp"

//----------------
// Public methods
//----------------

bool SettingsManager::begin() {
    LockGuard lock(mtx_);
    
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

const Settings& SettingsManager::getAllSettings() const {
LockGuard lock(mtx_);
return ramSettings_;
}

const PedalSettings& SettingsManager::getPedalSettings(size_t pedalIndex) const {
    LockGuard lock(mtx_);
    return ramSettings_.pedal[pedalIndex];
}

void SettingsManager::setPedalMode(size_t pedalIndex, PedalMode mode) {
    LockGuard lock(mtx_);
    if (pedalIndex >= MAX_PEDALS) return;

    if (ramSettings_.pedal[pedalIndex].pedalMode != mode) {
        ramSettings_.pedal[pedalIndex].pedalMode = mode;
        dirty_ = true;
        return;
    }
}

void SettingsManager::setMidiChannel(size_t pedalIndex, uint8_t ch) {
    LockGuard lock(mtx_);
    if (pedalIndex >= MAX_PEDALS) return;
    // 必要なら1〜16にクリップとかする
    if (ramSettings_.pedal[pedalIndex].midiChannel != ch) {
        ramSettings_.pedal[pedalIndex].midiChannel = ch;
        dirty_ = true;
        return;
    }
}

void SettingsManager::setCCNumber(size_t pedalIndex, uint8_t cc) {
    LockGuard lock(mtx_);
    if (pedalIndex >= MAX_PEDALS) return;

    if (ramSettings_.pedal[pedalIndex].ccNumber != cc) {
        ramSettings_.pedal[pedalIndex].ccNumber = cc;
        dirty_ = true;
        return;
    }
}

void SettingsManager::setSwitchBehavior(size_t pedalIndex, SwitchBehavior behavior) {
    LockGuard lock(mtx_);
    if (pedalIndex >= MAX_PEDALS) return;

    if (ramSettings_.pedal[pedalIndex].switchBehavior != behavior) {
        ramSettings_.pedal[pedalIndex].switchBehavior = behavior;
        dirty_ = true;
        return;
    }
}

void SettingsManager::setPedalSettings(size_t pedalIndex, const PedalSettings& ps) {
    LockGuard lock(mtx_);
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
    return;
}

void SettingsManager::setAllSettings(const Settings& s) {
    LockGuard lock(mtx_);
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
    return;
}

void SettingsManager::FactoryReset() {
    LockGuard lock(mtx_);
    loadFactoryDefaults();
    return;
}

bool SettingsManager::commitSettings() {
    LockGuard lock(mtx_);
    if (!dirty_) {
        return true; // 保存する変更なし
    }

    bool ok = writeToStorage();
    if (ok) {
        dirty_ = false;
    }
    return ok;
}

void SettingsManager::uncommitSettings() {
    LockGuard lock(mtx_);
    if (initialized_ == false || dirty_ == false) {
        return;
    }
    if (!loadFromStorage()) {
        loadFactoryDefaults();
    } 
    dirty_ = false;
    return;
}   

//------------------
// Private methods
//------------------

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
        ramSettings_.pedal[i].midiChannel    = 1;
        ramSettings_.pedal[i].ccNumber       = 1+i;
        ramSettings_.pedal[i].switchBehavior = SwitchBehavior::MOMENTARY;
        ramSettings_.pedal[i]._pad           = 0;
    }

    storage_.put(kStorageAddr, &ramSettings_, sizeof(Settings));
    storage_.commit();

    dirty_ = false;
    return;
}