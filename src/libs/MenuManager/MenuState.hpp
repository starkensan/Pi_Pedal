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

    enum ParamID : uint8_t
    {
        PARAM_NONE = 0,
        PARAM_BRIGHTNESS,
        PARAM_PEDAL_MIDI_MODE,
        PARAM_PEDAL_MIDI_CHANNEL,
        PARAM_PEDAL_CC_NUMBER,
        PARAM_PEDAL_SWITCH_MODE,
        PARAM_EXP_PEDAL_MIDI_CHANNEL,
        PARAM_EXP_PEDAL_CC_NUMBER,
        PRAM_ID_COUNT
    };

    enum MenuType : uint8_t
    {
        NONE = 0,
        SUBMENU,
        VALUE_CHANGE,
        FUNCTION,
        APLLY,
        SAVE,
        EXIT
    };

    struct ParamConfig
    {
        const ParamID paramID;
        int value;
        bool isLabelMode; // true: ラベル、false: 数値
        const int minValue;
        const int maxValue;
        const char* const* labels;

    };

    struct MenuItem
    {
        const char* label;
        const MenuType type;
        union {
            const MenuID submenuID;
            void (*actionFunc)();
            ParamID paramID;
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
    /**
     * @brief MenuID から対応する MenuConfig へのポインタを取得する
     * @param id 取得したいメニューの MenuID
     * @return 対応する MenuConfig へのポインタ（存在しない場合は nullptr）
     */
    const MenuConfig* getMenuConfig(MenuID id);

    /**
     * @brief ParamID から対応する ParamConfig へのポインタを取得する
     * @param id 取得したいパラメータの ParamID
     * @return 対応する ParamConfig へのポインタ（存在しない場合は nullptr）
     */
    const ParamConfig* getParamConfig(ParamID id);
    /**
     * @brief 指定した ParamID のパラメータ値を設定する
     * @param id 設定したいパラメータの ParamID
     * @param newValue 新しい値
     * @return true: 値が正常に設定された, false: 値が範囲外だった
     */
    bool setParamValue(ParamID id, int newValue);

} // namespace MenuState

#endif // MENU_STATE_HPP
