#ifndef MENU_MANAGER_HPP
#define MENU_MANAGER_HPP
#include "MenuState.hpp"

#include <config.h>
#include <HalStorage.hpp>

#include <SettingsManager/SettingsManager.hpp>
#include <MenuDisplay/MenuDisplay.hpp>

using namespace MenuState;

class MenuManager {
public:
    MenuManager(SettingsManager& settingsManager, HalDisplay& display)
    : menuDisplay_(display)
    , settingsManager_(settingsManager)
    {}
    ~MenuManager() = default;

    /**
     * @brief メニューシステムを初期化する
     * @param initMenu 初期メニュー設定
    */
    void init(MenuConfig& initMenu, TwoWire* wireInstance, int SDA, int SCL, int screenWidth, int screenHeight);
    
    /**
     * @brief 現在のメニュー設定を取得する
     * @return 現在のメニュー設定
    */
    MenuConfig getCurrentMenu() const;
    /**
     * @brief 選択されたメニュー項目に応じたアクションを実行する
    */
    void enterSelectedItem();
    /**
     * @brief 選択されたメニュー項目の値を変更する
     * @param newValue 新しい値
    */
    void cusorUp(int value = 1);
    void cusorDown(int value = 1);

    int getCurrentIndex() const { return index; }
    bool isSelected() const { return selected; }

private:

    void applySettings();

    void MemToParam();

    SettingsManager& settingsManager_;
    MenuDisplay menuDisplay_;

    MenuConfig currentMenu;
    bool selected;
    int index;
    int param;
    
};
#endif // MENU_MANAGER_HPP