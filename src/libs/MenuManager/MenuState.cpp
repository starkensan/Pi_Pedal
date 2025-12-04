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
        {"Save", MenuType::SAVE, .actionParam = {}, 0},
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
    char* PedalMidiModeLabels[] = {
        (char*)"CC",
        (char*)"PC NEXT",
        (char*)"PC BACK"
    };
    char* SwitchLabels[] = {
        (char*)"MOMENTARY",
        (char*)"TOGGLE"
    };

    ValueConfig PedalMidiModeValueConfig = {
        .valuePtr = nullptr,
        .isLabelled = true,
        .labels = PedalMidiModeLabels
    };
    ValueConfig PedalMidiChValueConfig = {
        .valuePtr = nullptr,
        .isLabelled = false,
        .labels = nullptr
    };
    ValueConfig PedalCCNumValueConfig = {
        .valuePtr = nullptr,
        .isLabelled = false,
        .labels = nullptr
    };
    ValueConfig PedalSwitchValueConfig = {
        .valuePtr = nullptr,
        .isLabelled = true,
        .labels = SwitchLabels
    };
    // Pedal Assignment Menu Items
    MenuItem PedalAssignmentMenuItems[] = {
        {"Mode", MenuType::VALUE_CHANGE, .actionParam = {.value = PedalMidiModeValueConfig}, 0},
        {"Midi Ch", MenuType::VALUE_CHANGE, .actionParam = {.value = PedalMidiChValueConfig}, 0},
        {"CC Num", MenuType::VALUE_CHANGE, .actionParam = {.value = PedalCCNumValueConfig}, 0},
        {"Switch", MenuType::VALUE_CHANGE, .actionParam = {.value = PedalSwitchValueConfig}, 0},
        {"Save", MenuType::SAVE, .actionParam = {}, 0},
        {"Exit", MenuType::EXIT, .actionParam = {}, 0}
    };
    MenuConfig PedalAssignmentMenuConfig = {
        MenuID::PEDAL_ASSIGNMENT,
        "Pedal Assignment",
        PedalAssignmentMenuItems,
        static_cast<uint8_t>(sizeof(PedalAssignmentMenuItems) / sizeof(PedalAssignmentMenuItems[0])),
        MenuID::PEDAL_SETTINGS
    };

    // Exp Pedal Assignment Menu Items
    ValueConfig ExpPedalMidiChValueConfig = {
        .valuePtr = nullptr,
        .isLabelled = false,
        .labels = nullptr
    };
    ValueConfig ExpPedalCCNumValueConfig = {
        .valuePtr = nullptr,
        .isLabelled = false,
        .labels = nullptr
    };
    MenuItem ExpPedalAssignmentMenuItems[] = {
        {"Midi Ch", MenuType::VALUE_CHANGE, .actionParam = {.value = ExpPedalMidiChValueConfig}, 0},
        {"CC Num", MenuType::VALUE_CHANGE, .actionParam = {.value = ExpPedalCCNumValueConfig}, 0},
        {"Exit", MenuType::EXIT, .actionParam = {}, 0}
    };
    MenuConfig ExpPedalAssignmentMenuConfig = {
        MenuID::EXP_PEDAL_ASSIGNMENT,
        "Exp Pedal Assign",
        ExpPedalAssignmentMenuItems,
        static_cast<uint8_t>(sizeof(ExpPedalAssignmentMenuItems) / sizeof(ExpPedalAssignmentMenuItems[0])),
        MenuID::EXP_PEDAL_SETTINGS
    };

    ValueConfig BrigthnessValueConfig = {
        .valuePtr = nullptr,
        .isLabelled = false,
        .labels = nullptr
    };

    // Display Settings Menu Items
    MenuItem DisplaySettingsMenuItems[] = {
        {"Brightness", MenuType::VALUE_CHANGE, .actionParam = {.value = BrigthnessValueConfig}, 0},
        {"Exit", MenuType::EXIT, .actionParam = {}, 0}
    };
    MenuConfig DisplaySettingsMenuConfig = {
        MenuID::DISPLAY_SETTINGS,
        "Display Settings",
        DisplaySettingsMenuItems,
        static_cast<uint8_t>(sizeof(DisplaySettingsMenuItems) / sizeof(DisplaySettingsMenuItems[0])),
        MenuID::SETTINGS
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
        // MAIN はメニュー画面にしないなら省略してOK
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

} // namespace MenuState
