#pragma once

#include <SettingsManager/SettingsStore.hpp>

class SettingsStoreMock : public SettingsStore {
public:
    SettingsStoreMock()
        : commitReturnValue(true),
          commitCalledCount(0),
          factoryResetCalled(false),
          uncommitCalled(false),
          dirty(false)
    {
        settings.version = 1;
        for (size_t i = 0; i < MAX_PEDALS; ++i) {
            settings.pedal[i].pedalMode = PedalMode::CC;
            settings.pedal[i].midiChannel = 1;
            settings.pedal[i].ccNumber = i + 1;
            settings.pedal[i].switchBehavior = SwitchBehavior::MOMENTARY;
            settings.pedal[i]._pad = 0;
        }
    }

    const Settings& getAllSettings() const override {
        return settings;
    }

    const PedalSettings& getPedalSettings(size_t pedalIndex) const override {
        return settings.pedal[pedalIndex];
    }

    void setPedalMode(size_t pedalIndex, PedalMode mode) override {
        if (pedalIndex >= MAX_PEDALS) return;
        settings.pedal[pedalIndex].pedalMode = mode;
        dirty = true;
    }

    void setMidiChannel(size_t pedalIndex, uint8_t ch) override {
        if (pedalIndex >= MAX_PEDALS) return;
        settings.pedal[pedalIndex].midiChannel = ch;
        dirty = true;
    }

    void setCCNumber(size_t pedalIndex, uint8_t cc) override {
        if (pedalIndex >= MAX_PEDALS) return;
        settings.pedal[pedalIndex].ccNumber = cc;
        dirty = true;
    }

    void setSwitchBehavior(size_t pedalIndex, SwitchBehavior behavior) override {
        if (pedalIndex >= MAX_PEDALS) return;
        settings.pedal[pedalIndex].switchBehavior = behavior;
        dirty = true;
    }

    void setPedalSettings(size_t pedalIndex, const PedalSettings& ps) override {
        if (pedalIndex >= MAX_PEDALS) return;
        settings.pedal[pedalIndex] = ps;
        dirty = true;
    }

    void setAllSettings(const Settings& s) override {
        settings = s;
        dirty = true;
    }

    bool commitSettings() override {
        commitCalledCount++;
        if (commitReturnValue) {
            dirty = false;
        }
        return commitReturnValue;
    }

    void uncommitSettings() override {
        uncommitCalled = true;
        dirty = false;
    }

    void FactoryReset() override {
        factoryResetCalled = true;
        dirty = false;
    }

    bool getIsDirty() const override {
        return dirty;
    }

    Settings settings;
    bool commitReturnValue;
    int commitCalledCount;
    bool factoryResetCalled;
    bool uncommitCalled;
    bool dirty;
};
