#ifndef MENU_MANAGER_HPP
#define MENU_MANAGER_HPP
#include "MenuState.hpp"

#include <config.h>
#include <HalStorage.hpp>

#include <SettingsManager/SettingsStore.hpp>
#include <MenuDisplay/MenuDisplay.hpp>

using namespace MenuState;

class MenuManager {
public:
    MenuManager(SettingsStore& settingsStore, HalDisplay& display)
    : menuDisplay_(display)
    , settingsStore_(settingsStore)
    {}
    ~MenuManager() = default;

    /**
     * @brief メニューシステムを初期化する
     * @param initMenu 初期メニュー設定
    */
    void init(MenuID initMenuID);
    
    /**
     * @brief 現在のメニュー設定を取得する
     * @return 現在のメニュー設定
    */
    MenuConfig getCurrentMenu() const;
    /**
     * @brief PC番号を表示する（メインメニュー時のみ）
     * @param display HalDisplayインスタンス
     * @param number 表示するPC番号
    */
    void showPCNumber(int number);
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
    bool isSetting() const { return currentMenu.menuID != MenuID::MAIN; }

private:

    void applySettings();

    void MemToParam();

    SettingsStore& settingsStore_;
    MenuDisplay menuDisplay_;

    MenuConfig currentMenu;
    bool selected;
    int index;
    int param;

    int PCNumber = 0;
    
};
#endif // MENU_MANAGER_HPP
