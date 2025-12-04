// MenuState.hpp
#ifndef MENU_STATE_HPP
#define MENU_STATE_HPP

#include <stdint.h>

namespace MenuState {

    enum MenuID : uint8_t
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

    enum MenuType : uint8_t
    {
        NONE = 0,
        SUBMENU,
        VALUE_CHANGE,
        FUNCTION,
        SAVE,
        EXIT
    };

    struct ValueConfig
    {
        int* valuePtr;
        bool isLabelled;
        const char* const* labels;

    };

    struct MenuItem
    {
        const char* label;
        const MenuType type;
        union {
            const MenuID submenuID;
            void (*actionFunc)();
            ValueConfig value;
        } actionParam;
        int param;
    };

    struct MenuConfig
    {
        MenuID menuID;
        const char* title;
        const MenuItem* items;  // 配列へのポインタ
        uint8_t itemCount;
        MenuID prevMenuID;
    };

    // ===== メニュー配列・Config の「宣言」だけ =====

    // 例：Settings メニュー
    extern const MenuItem SettingMenuItems[];
    extern const MenuConfig SettingMenuConfig;

    // 他のメニューも必要に応じて宣言
    extern const MenuItem PedalSettingsMenuItems[];
    extern const MenuConfig PedalSettingsMenuConfig;

    extern const MenuItem ExpPedalSettingsMenuItems[];
    extern const MenuConfig ExpPedalSettingsMenuConfig;

    extern const MenuItem DisplaySettingsMenuItems[];
    extern const MenuConfig DisplaySettingsMenuConfig;

    extern const MenuItem PedalAssignmentMenuItems[];
    extern const MenuConfig PedalAssignmentMenuConfig;

    extern const MenuItem ExpPedalAssignmentMenuItems[];
    extern const MenuConfig ExpPedalAssignmentMenuConfig;

    extern const MenuItem ExpPedalCalibrationMenuItems[];
    extern const MenuConfig ExpPedalCalibrationMenuConfig;

    // MenuID → MenuConfig* を引くためのテーブル
    extern const MenuConfig* const MenuTable[MENU_ID_COUNT - 1]; 
    // （MAIN を除く分だけ、みたいなイメージ）

    // API
    const MenuConfig* getMenuConfig(MenuID id);

} // namespace MenuState

#endif // MENU_STATE_HPP
