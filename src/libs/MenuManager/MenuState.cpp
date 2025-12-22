// MenuState.cpp
#include "MenuState.hpp"

namespace MenuState {

    static int g_paramValues[static_cast<uint8_t>(ParamID::PRAM_ID_COUNT)];

    //===========================
    // Menu Items 実体
    //===========================

    // Settings メニュー項目
    const MenuItem SettingMenuItems[] = {
        {"Pedal",   MenuType::SUBMENU, {.submenuID = MenuID::PEDAL_SETTINGS},      0},
        {"Exp",     MenuType::SUBMENU, {.submenuID = MenuID::EXP_PEDAL_SETTINGS},  0},
        {"Display", MenuType::SUBMENU, {.submenuID = MenuID::DISPLAY_SETTINGS},    0},
        {"Save",    MenuType::SAVE,    {},                                         0},
        {"Exit",    MenuType::EXIT,    {},                                         0}
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
        {"Exit",    MenuType::EXIT,    {},                                      0}
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
        {"Assign", MenuType::SUBMENU, {.submenuID = MenuID::EXP_PEDAL_ASSIGNMENT},  0},
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
        {"Calib", MenuType::FUNCTION,   .actionParam = {.actionFunc = nullptr}, 0},
        {"Apply", MenuType::APPLY,      .actionParam = {},                      0},
        {"Exit",    MenuType::EXIT,     .actionParam = {},                      0}
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
        {"Mode",    MenuType::VALUE_CHANGE, .actionParam = {.paramID = ParamID::PARAM_PEDAL_MIDI_MODE},     0},
        {"Ch",      MenuType::VALUE_CHANGE, .actionParam = {.paramID = ParamID::PARAM_PEDAL_MIDI_CHANNEL},  0},
        {"Num",     MenuType::VALUE_CHANGE, .actionParam = {.paramID = ParamID::PARAM_PEDAL_CC_NUMBER},     0},
        {"SW",      MenuType::VALUE_CHANGE, .actionParam = {.paramID = ParamID::PARAM_PEDAL_SWITCH_MODE},   0},
        {"Apply",   MenuType::APPLY,        .actionParam = {},                                              0},
        {"Exit",    MenuType::EXIT,         .actionParam = {},  0}
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
        {"Ch",      MenuType::VALUE_CHANGE, .actionParam = {.paramID = ParamID::PARAM_EXP_PEDAL_MIDI_CHANNEL},  0},
        {"Num",     MenuType::VALUE_CHANGE, .actionParam = {.paramID = ParamID::PARAM_EXP_PEDAL_CC_NUMBER},     0},
        {"Apply",   MenuType::APPLY,        .actionParam = {},                                                  0},
        {"Exit",    MenuType::EXIT,         .actionParam = {},                                                  0}
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
        {"Level", MenuType::VALUE_CHANGE, .actionParam = {.paramID = ParamID::PARAM_BRIGHTNESS}, 0},
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
        (char*)"NEXT",
        (char*)"BACK"
    };
    const char* SwitchLabels[] = {
        (char*)"MEM",
        (char*)"TGL"
    };

    // ParamConfig 実体

    const ParamConfig PedalMidiModeParamConfig = {
        .paramID = ParamID::PARAM_PEDAL_MIDI_MODE,
        .defaultValue = 0,
        .isLabelMode = true,
        .minValue = 0,
        .maxValue = static_cast<int>(sizeof(PedalMidiModeLabels) / sizeof(PedalMidiModeLabels[0])) - 1,
        .labels = PedalMidiModeLabels
    };
    const ParamConfig PedalMidiChParamConfig = {
        .paramID = ParamID::PARAM_PEDAL_MIDI_CHANNEL,
        .defaultValue = 1,
        .isLabelMode = false,
        .minValue = 1,
        .maxValue = 16,
        .labels = nullptr
    };
    const ParamConfig PedalCCNumParamConfig = {
        .paramID = ParamID::PARAM_PEDAL_CC_NUMBER,
        .defaultValue = 0,
        .isLabelMode = false,
        .minValue = 0,
        .maxValue = 127,
        .labels = nullptr
    };
    const ParamConfig PedalSwitchParamConfig = {
        .paramID = ParamID::PARAM_PEDAL_SWITCH_MODE,
        .defaultValue = 0,
        .isLabelMode = true,
        .minValue = 0,
        .maxValue = static_cast<int>(sizeof(SwitchLabels) / sizeof(SwitchLabels[0])) - 1,
        .labels = SwitchLabels
    };

    const ParamConfig ExpPedalMidiChParamConfig = {
        .paramID = ParamID::PARAM_EXP_PEDAL_MIDI_CHANNEL,
        .defaultValue = 1,
        .isLabelMode = false,
        .minValue = 1,
        .maxValue = 16,
        .labels = nullptr
    };
    const ParamConfig ExpPedalCCNumParamConfig = {
        .paramID = ParamID::PARAM_EXP_PEDAL_CC_NUMBER,
        .defaultValue = 0,
        .isLabelMode = false,
        .minValue = 0,
        .maxValue = 127,
        .labels = nullptr
    };

    const ParamConfig BrightnessParamConfig = {
        .paramID = ParamID::PARAM_BRIGHTNESS,
        .defaultValue = 0,
        .isLabelMode = false,
        .minValue = 0,
        .maxValue = 100,
        .labels = nullptr
    };

    //===========================
    // ParamTable 実体
    //===========================
    const ParamConfig* ParamTable[PRAM_ID_COUNT - 1] = {
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
    // initParamsOnce 実装
    //===========================

    void initParamsOnce() {
        static bool inited = false;
        if (inited) return;
        inited = true;

        for (const auto& c : ParamTable) {
            g_paramValues[static_cast<uint8_t>(c->paramID)] = c->defaultValue;
        }
    }

    //===========================
    // getParamValue 実装
    //===========================

    int getParamValue(ParamID id)
    {
        initParamsOnce();
        return g_paramValues[static_cast<uint8_t>(id)];
    }

    //===========================
    // setValue 実装
    //===========================

    bool setParamValue(ParamID id, int newValue)
    {
        initParamsOnce();
        for (uint8_t i = 0; i < PRAM_ID_COUNT - 1; ++i) {
            if (ParamTable[i]->paramID == id) {
                if (newValue < ParamTable[i]->minValue || newValue > ParamTable[i]->maxValue)
                {
                    return false;; // 範囲外
                }
                
                g_paramValues[id] = newValue;
                return true; // 正常に設定された
            }
        }
        return false; // 見つからなかった
    }
} // namespace MenuState

