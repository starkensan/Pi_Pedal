// MenuState.cpp
#include "MenuState.hpp"

namespace MenuState {

    //===========================
    // Menu Items 実体
    //===========================

    // Settings メニュー項目
    const MenuItem SettingMenuItems[] = {
        {"Pedal Settings",     MenuType::SUBMENU, {.submenuID = MenuID::PEDAL_SETTINGS},      0},
        {"Exp Pedal Settings", MenuType::SUBMENU, {.submenuID = MenuID::EXP_PEDAL_SETTINGS},  0},
        {"Display Settings",   MenuType::SUBMENU, {.submenuID = MenuID::DISPLAY_SETTINGS},    0},
        {"Save",               MenuType::SAVE,     {},                                         0},
        {"Exit",               MenuType::EXIT,    {},                                         0}
    };

    const MenuConfig SettingMenuConfig = {
        MenuID::SETTINGS,
        "Settings",
        SettingMenuItems,
        static_cast<uint8_t>(sizeof(SettingMenuItems) / sizeof(SettingMenuItems[0])),
        MenuID::MAIN
    };

    // Pedal Settings メニュー項目
    const MenuItem PedalSettingsMenuItems[] = {
        {"Pedal 1", MenuType::SUBMENU, {.submenuID = MenuID::PEDAL_ASSIGNMENT}, 1},
        {"Pedal 2", MenuType::SUBMENU, {.submenuID = MenuID::PEDAL_ASSIGNMENT}, 2},
        {"Pedal 3", MenuType::SUBMENU, {.submenuID = MenuID::PEDAL_ASSIGNMENT}, 3},
        {"Pedal 4", MenuType::SUBMENU, {.submenuID = MenuID::PEDAL_ASSIGNMENT}, 4},
        {"Pedal 5", MenuType::SUBMENU, {.submenuID = MenuID::PEDAL_ASSIGNMENT}, 5},
        {"Pedal 6", MenuType::SUBMENU, {.submenuID = MenuID::PEDAL_ASSIGNMENT}, 6},
        {"Exit",    MenuType::EXIT,    {},                                       0}
    };

    const MenuConfig PedalSettingsMenuConfig = {
        MenuID::PEDAL_SETTINGS,
        "Pedal Settings",
        PedalSettingsMenuItems,
        static_cast<uint8_t>(sizeof(PedalSettingsMenuItems) / sizeof(PedalSettingsMenuItems[0])),
        MenuID::SETTINGS
    };

    // Exp Pedal Settings
    const MenuItem ExpPedalSettingsMenuItems[] = {
        {"Assign", MenuType::SUBMENU, {.submenuID = MenuID::EXP_PEDAL_ASSIGNMENT}, 0},
        {"Calib",  MenuType::SUBMENU, {.submenuID = MenuID::EXP_PEDAL_CALIBRATION}, 0},
        {"Exit",   MenuType::EXIT,    {},                                           0}
    };

    const MenuConfig ExpPedalSettingsMenuConfig = {
        MenuID::EXP_PEDAL_SETTINGS,
        "Exp Pedal Settings",
        ExpPedalSettingsMenuItems,
        static_cast<uint8_t>(sizeof(ExpPedalSettingsMenuItems) / sizeof(ExpPedalSettingsMenuItems[0])),
        MenuID::SETTINGS
    };

    // Exp Pedal Calibration Menu Items
    const MenuItem ExpPedalCalibrationMenuItems[] = {
        {"Calibrate", MenuType::FUNCTION, .actionParam = {.actionFunc = nullptr}, 0},
        {"Save", MenuType::APLLY, .actionParam = {}, 0},
        {"Exit", MenuType::EXIT, .actionParam = {}, 0}
    };
    const MenuConfig ExpPedalCalibrationMenuConfig = {
        MenuID::EXP_PEDAL_CALIBRATION,
        "Exp Pedal Calib",
        ExpPedalCalibrationMenuItems,
        static_cast<uint8_t>(sizeof(ExpPedalCalibrationMenuItems) / sizeof(ExpPedalCalibrationMenuItems[0])),
        MenuID::EXP_PEDAL_SETTINGS
    };
    // Pedal Assignment Menu Items
    const MenuItem PedalAssignmentMenuItems[] = {
        {"Mode", MenuType::VALUE_CHANGE, .actionParam = {.paramID = ParamID::PARAM_PEDAL_MIDI_MODE}, 0},
        {"Midi Ch", MenuType::VALUE_CHANGE, .actionParam = {.paramID = ParamID::PARAM_PEDAL_MIDI_CHANNEL}, 0},
        {"CC Num", MenuType::VALUE_CHANGE, .actionParam = {.paramID = ParamID::PARAM_PEDAL_CC_NUMBER}, 0},
        {"Switch", MenuType::VALUE_CHANGE, .actionParam = {.paramID = ParamID::PARAM_PEDAL_SWITCH_MODE}, 0},
        {"Save", MenuType::APLLY, .actionParam = {}, 0},
        {"Exit", MenuType::EXIT, .actionParam = {}, 0}
    };
    const MenuConfig PedalAssignmentMenuConfig = {
        MenuID::PEDAL_ASSIGNMENT,
        "Pedal Assignment",
        PedalAssignmentMenuItems,
        static_cast<uint8_t>(sizeof(PedalAssignmentMenuItems) / sizeof(PedalAssignmentMenuItems[0])),
        MenuID::PEDAL_SETTINGS
    };

    // Exp Pedal Assignment Menu Items
    const MenuItem ExpPedalAssignmentMenuItems[] = {
        {"Midi Ch", MenuType::VALUE_CHANGE, .actionParam = {.paramID = ParamID::PARAM_EXP_PEDAL_MIDI_CHANNEL}, 0},
        {"CC Num", MenuType::VALUE_CHANGE, .actionParam = {.paramID = ParamID::PARAM_EXP_PEDAL_CC_NUMBER}, 0},
        {"Exit", MenuType::EXIT, .actionParam = {}, 0}
    };
    const MenuConfig ExpPedalAssignmentMenuConfig = {
        MenuID::EXP_PEDAL_ASSIGNMENT,
        "Exp Pedal Assign",
        ExpPedalAssignmentMenuItems,
        static_cast<uint8_t>(sizeof(ExpPedalAssignmentMenuItems) / sizeof(ExpPedalAssignmentMenuItems[0])),
        MenuID::EXP_PEDAL_SETTINGS
    };

    // Display Settings Menu Items
    const MenuItem DisplaySettingsMenuItems[] = {
        {"Brightness", MenuType::VALUE_CHANGE, .actionParam = {.paramID = ParamID::PARAM_BRIGHTNESS}, 0},
        {"Exit", MenuType::EXIT, .actionParam = {}, 0}
    };
    const MenuConfig DisplaySettingsMenuConfig = {
        MenuID::DISPLAY_SETTINGS,
        "Display Settings",
        DisplaySettingsMenuItems,
        static_cast<uint8_t>(sizeof(DisplaySettingsMenuItems) / sizeof(DisplaySettingsMenuItems[0])),
        MenuID::SETTINGS
    };

    // Pedal Assignment Menu Items
    const char* PedalMidiModeLabels[] = {
        (char*)"CC",
        (char*)"PC NEXT",
        (char*)"PC BACK"
    };
    const char* SwitchLabels[] = {
        (char*)"MOMENTARY",
        (char*)"TOGGLE"
    };

    // ParamConfig 実体

    ParamConfig PedalMidiModeParamConfig = {
        .paramID = ParamID::PARAM_PEDAL_MIDI_MODE,
        .value = 0,
        .isLabelMode = true,
        .minValue = 0,
        .maxValue = static_cast<int>(sizeof(PedalMidiModeLabels) / sizeof(PedalMidiModeLabels[0])) - 1,
        .labels = PedalMidiModeLabels
    };
    ParamConfig PedalMidiChParamConfig = {
        .paramID = ParamID::PARAM_PEDAL_MIDI_CHANNEL,
        .value = 1,
        .isLabelMode = false,
        .minValue = 1,
        .maxValue = 16,
        .labels = nullptr
    };
    ParamConfig PedalCCNumParamConfig = {
        .paramID = ParamID::PARAM_PEDAL_CC_NUMBER,
        .value = 0,
        .isLabelMode = false,
        .minValue = 0,
        .maxValue = 127,
        .labels = nullptr
    };
    ParamConfig PedalSwitchParamConfig = {
        .paramID = ParamID::PARAM_PEDAL_SWITCH_MODE,
        .value = 0,
        .isLabelMode = true,
        .minValue = 0,
        .maxValue = static_cast<int>(sizeof(SwitchLabels) / sizeof(SwitchLabels[0])) - 1,
        .labels = SwitchLabels
    };

    ParamConfig ExpPedalMidiChParamConfig = {
        .paramID = ParamID::PARAM_EXP_PEDAL_MIDI_CHANNEL,
        .value = 1,
        .isLabelMode = false,
        .minValue = 1,
        .maxValue = 16,
        .labels = nullptr
    };
    ParamConfig ExpPedalCCNumParamConfig = {
        .paramID = ParamID::PARAM_EXP_PEDAL_CC_NUMBER,
        .value = 0,
        .isLabelMode = false,
        .minValue = 0,
        .maxValue = 127,
        .labels = nullptr
    };

    ParamConfig BrightnessParamConfig = {
        .paramID = ParamID::PARAM_BRIGHTNESS,
        .value = 0,
        .isLabelMode = false,
        .minValue = 0,
        .maxValue = 100,
        .labels = nullptr
    };

    //===========================
    // ParamTable 実体
    //===========================
    ParamConfig* ParamTable[PRAM_ID_COUNT - 1] = {
        &PedalMidiModeParamConfig,
        &PedalMidiChParamConfig,
        &PedalCCNumParamConfig,
        &PedalSwitchParamConfig,
        &ExpPedalMidiChParamConfig,
        &ExpPedalCCNumParamConfig,
        &BrightnessParamConfig
    };


    //===========================
    // MenuTable 実体
    //===========================

    const MenuConfig* const MenuTable[MENU_ID_COUNT-1] = {
        &SettingMenuConfig,
        &PedalSettingsMenuConfig,
        &ExpPedalSettingsMenuConfig,
        &DisplaySettingsMenuConfig,
        &PedalAssignmentMenuConfig,
        &ExpPedalAssignmentMenuConfig,
        &ExpPedalCalibrationMenuConfig
    };

    //===========================
    // getMenuConfig 実装
    //===========================

    const MenuConfig* getMenuConfig(MenuID id)
    {
        for (uint8_t i = 0; i < MENU_ID_COUNT - 1; ++i) {
            if (MenuTable[i]->menuID == id) {
                return MenuTable[i];
            }
        }
        return nullptr;
    }

    //===========================
    // getParamConfig 実装
    //===========================

    const ParamConfig* getParamConfig(ParamID id)
    {
        for (uint8_t i = 0; i < PRAM_ID_COUNT - 1; ++i) {
            if (ParamTable[i]->paramID == id) {
                return ParamTable[i];
            }
        }
        return nullptr;
    }

    //===========================
    // setValue 実装
    //===========================

    bool setParamValue(ParamID id, int newValue)
    {
        for (uint8_t i = 0; i < PRAM_ID_COUNT - 1; ++i) {
            if (ParamTable[i]->paramID == id) {
                if (newValue < ParamTable[i]->minValue || newValue > ParamTable[i]->maxValue)
                {
                    return false;; // 範囲外
                }
                
                ParamTable[i]->value = newValue;
                return true; // 正常に設定された
            }
        }
        return false; // 見つからなかった
    }
} // namespace MenuState
