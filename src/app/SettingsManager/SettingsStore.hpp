#ifndef SETTINGSSTORE_HPP_
#define SETTINGSSTORE_HPP_

#include <stddef.h>
#include "SettingsDefs.hpp"

using namespace SettingsDefs;

class SettingsStore {
public:
    virtual ~SettingsStore() {}

    virtual const Settings& getAllSettings() const = 0;
    virtual const PedalSettings& getPedalSettings(size_t pedalIndex) const = 0;

    virtual void setPedalMode(size_t pedalIndex, PedalMode mode) = 0;
    virtual void setMidiChannel(size_t pedalIndex, uint8_t ch) = 0;
    virtual void setCCNumber(size_t pedalIndex, uint8_t cc) = 0;
    virtual void setSwitchBehavior(size_t pedalIndex, SwitchBehavior behavior) = 0;
    virtual void setPedalSettings(size_t pedalIndex, const PedalSettings& ps) = 0;
    virtual void setAllSettings(const Settings& s) = 0;

    virtual bool commitSettings() = 0;
    virtual void uncommitSettings() = 0;
    virtual void FactoryReset() = 0;
    virtual bool getIsDirty() const = 0;
};

#endif // SETTINGSSTORE_HPP_
