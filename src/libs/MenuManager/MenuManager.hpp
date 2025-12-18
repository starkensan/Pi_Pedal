#ifndef MENU_MANAGER_HPP
#define MENU_MANAGER_HPP
#include "MenuState.hpp"

using namespace MenuState;

class MenuManager {
public:
    MenuManager() = default;
    ~MenuManager() = default;

    /**
     * @brief メニューシステムを初期化する
     * @param initMenu 初期メニュー設定
    */
    void init(MenuConfig& initMenu);
    
    /**
     * @brief 現在のメニュー設定を取得する
     * @return 現在のメニュー設定
    */
    MenuConfig getCurrentMenu() const;
    /**
     * @brief 選択されたメニュー項目に応じたアクションを実行する
     * @param selectedIndex 選択されたメニュー項目のインデックス
    */
    void enterSelectedItem(uint8_t selectedIndex);
    /**
     * @brief 選択されたメニュー項目の値を変更する
     * @param selectedIndex 選択されたメニュー項目のインデックス
     * @param newValue 新しい値
    */
    void changeValue(uint8_t selectedIndex, int newValue);
    /**
     * @brief メニューが値変更モードかどうかを取得する
     * @return 値変更モードならtrue、そうでなければfalse
    */
    bool isSelected() const { return selected; }

private:

    MenuConfig currentMenu;
    bool selected;
    int param;
    
};
#endif // MENU_MANAGER_HPP