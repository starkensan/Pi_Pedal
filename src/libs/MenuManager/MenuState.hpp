#ifndef MENU_STATE_HPP
#define MENU_STATE_HPP
#include <stdint.h>

class MenuState {
public:
    enum MenuID
    {
        MAIN = 0,
        SETTINGS,
        // Settings Submenus
        PEDAL_SETTINGS,
        EXP_PEDAL_SETTINGS,
        DISPLAY_SETTINGS,
        // PEDAL SETTINGS Submenus
        PEDAL_ASSIGNMENT,
        // EXP PEDAL SETTINGS Submenus
        EXP_PEDAL_ASSIGNMENT,
        EXP_PEDAL_CALIBRATION,
        MENU_ID_COUNT

    };

    enum MenuType
    {
        NONE = 0,
        SUBMENU,
        VALUE_CHANGE,
        FUNCTION,
        SAVE,
        EXIT
    };

    struct MenuItem
    {
        const char* label;
        MenuType type;
        union
        {
            MenuID submenuID;
            void (*actionFunc)();
            int* valuePtr;
        } actionParam;
        int param;
    };

    struct MenuConfig
    {
        MenuID id;
        const char* title;
        MenuItem* items;
        uint8_t itemCount;
    };

    //===========================
    // Menu Items Definitions
    //===========================

    // Main Menu Items
    MenuItem SettingMenuItems[4] = {
        {"Pedal Settings", MenuType::SUBMENU, .actionParam = {.submenuID = MenuID::PEDAL_SETTINGS}, 0},
        {"Exp Pedal Settings", MenuType::SUBMENU, .actionParam = {.submenuID = MenuID::EXP_PEDAL_SETTINGS}, 0},
        {"Display Settings", MenuType::SUBMENU, .actionParam = {.submenuID = MenuID::DISPLAY_SETTINGS}, 0},
        {"Exit", MenuType::EXIT, .actionParam = {}, 0}
    };

    MenuConfig SettingMenuConfig = {
        MenuID::SETTINGS,
        "Settings",
        SettingMenuItems,
        4
    };


    // Pedal Settings Menu Items
    MenuItem PedalSettingsMenuItems[7] = {
        {"Pedal", MenuType::SUBMENU, .actionParam = {.submenuID = MenuID::PEDAL_ASSIGNMENT}, 1},
        {"Pedal", MenuType::SUBMENU, .actionParam = {.submenuID = MenuID::PEDAL_ASSIGNMENT}, 2},
        {"Pedal", MenuType::SUBMENU, .actionParam = {.submenuID = MenuID::PEDAL_ASSIGNMENT}, 3},
        {"Pedal", MenuType::SUBMENU, .actionParam = {.submenuID = MenuID::PEDAL_ASSIGNMENT}, 4},
        {"Pedal", MenuType::SUBMENU, .actionParam = {.submenuID = MenuID::PEDAL_ASSIGNMENT}, 5},
        {"Pedal", MenuType::SUBMENU, .actionParam = {.submenuID = MenuID::PEDAL_ASSIGNMENT}, 6},
        {"Exit", MenuType::EXIT, .actionParam = {}, 0}
    };

    MenuConfig PedalSettingsMenuConfig = {
        MenuID::PEDAL_SETTINGS,
        "Pedal Settings",
        PedalSettingsMenuItems,
        7
    };

    // Exp Pedal Settings Menu Items
    MenuItem ExpPedalSettingsMenuItems[3] = {
        {"Assign", MenuType::SUBMENU, .actionParam = {.submenuID = MenuID::EXP_PEDAL_ASSIGNMENT}, 0},
        {"Calib", MenuType::SUBMENU, .actionParam = {.submenuID = MenuID::EXP_PEDAL_CALIBRATION}, 0},
        {"Exit", MenuType::EXIT, .actionParam = {}, 0}
    };
    MenuConfig ExpPedalSettingsMenuConfig = {
        MenuID::EXP_PEDAL_SETTINGS,
        "Exp Pedal Settings",
        ExpPedalSettingsMenuItems,
        3
    };

    // Display Settings Menu Items
    MenuItem DisplaySettingsMenuItems[2] = {
        {"Brightness", MenuType::NONE, .actionParam = {}, 0},
        {"Exit", MenuType::EXIT, .actionParam = {}, 0}
    };
    MenuConfig DisplaySettingsMenuConfig = {
        MenuID::DISPLAY_SETTINGS,
        "Display Settings",
        DisplaySettingsMenuItems,
        2
    };

    // Pedal Assignment Menu Items
    MenuItem PedalAssignmentMenuItems[8] = {
        {"Midi mode", MenuType::VALUE_CHANGE, .actionParam = {}, 0},
        {"Midi Ch", MenuType::VALUE_CHANGE, .actionParam = {}, 0},
        {"CC Num", MenuType::VALUE_CHANGE, .actionParam = {}, 0},
        {"Switch", MenuType::VALUE_CHANGE, .actionParam = {}, 0},
        {"Save", MenuType::SAVE, .actionParam = {}, 0},
        {"Exit", MenuType::EXIT, .actionParam = {}, 0}
    };
    MenuConfig PedalAssignmentMenuConfig = {
        MenuID::PEDAL_ASSIGNMENT,
        "Pedal Assignment",
        PedalAssignmentMenuItems,
        6
    };

    // Exp Pedal Assignment Menu Items
    MenuItem ExpPedalAssignmentMenuItems[3] = {
        {"Midi Ch", MenuType::VALUE_CHANGE, .actionParam = {}, 0},
        {"CC Num", MenuType::VALUE_CHANGE, .actionParam = {}, 0},
        {"Exit", MenuType::EXIT, .actionParam = {}, 0}
    };
    MenuConfig ExpPedalAssignmentMenuConfig = {
        MenuID::EXP_PEDAL_ASSIGNMENT,
        "Exp Pedal Assign",
        ExpPedalAssignmentMenuItems,
        3
    };

    // Exp Pedal Calibration Menu Items
    MenuItem ExpPedalCalibrationMenuItems[3] = {
        {"Calibrate", MenuType::FUNCTION, .actionParam = {.actionFunc = nullptr}, 0},
        {"Save", MenuType::SAVE, .actionParam = {}, 0},
        {"Exit", MenuType::EXIT, .actionParam = {}, 0}
    };
    MenuConfig ExpPedalCalibrationMenuConfig = {
        MenuID::EXP_PEDAL_CALIBRATION,
        "Exp Pedal Calib",
        ExpPedalCalibrationMenuItems,
        3
    };

    MenuState();

    MenuConfig* getMenuConfig(MenuID id){
        for (uint8_t i = 0; i < static_cast<uint8_t>(MenuID::MENU_ID_COUNT); ++i) {
            if (MenuTable[i].id == id) {
                return &MenuTable[i];
            }
        }
        return nullptr; // Not found
    }
private:
    MenuConfig MenuTable[static_cast<uint8_t>(MenuID::MENU_ID_COUNT)] = {
        // MENU_ID_MAIN is not used here
        SettingMenuConfig,
        PedalSettingsMenuConfig,
        ExpPedalSettingsMenuConfig,
        DisplaySettingsMenuConfig,
        PedalAssignmentMenuConfig,
        ExpPedalAssignmentMenuConfig,
        ExpPedalCalibrationMenuConfig
    };

};

#endif // MENU_STATE_HPP