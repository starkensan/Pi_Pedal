#include "MenuManager.hpp"
void MenuManager::init(MenuConfig& initMenu) {
    currentMenu = initMenu;
    selected = false;
    param = 0;
}

MenuConfig MenuManager::getCurrentMenu() const {
    return currentMenu;
}

void MenuManager::enterSelectedItem(uint8_t selectedIndex) {
    if (selectedIndex >= currentMenu.itemCount) {
        return; // 無効なインデックス
    }

    MenuItem selectedItem = currentMenu.items[selectedIndex];
    switch (selectedItem.type) {
        case MenuType::SUBMENU: {
            // サブメニューに移動
            currentMenu = *getMenuConfig(selectedItem.actionParam.submenuID);
            break;
        }
        case MenuType::SAVE: {
            // 保存処理（必要に応じて実装）
            break;
        }
        case MenuType::EXIT: {
            // 前のメニューに戻る
            currentMenu = *getMenuConfig(currentMenu.prevMenuID);
            break;
        }
        case MenuType::FUNCTION: {
            // 関数を呼び出す
            if (selectedItem.actionParam.actionFunc != nullptr) {
                selectedItem.actionParam.actionFunc();
            }
            break;
        }
        case MenuType::VALUE_CHANGE: {
            // 値変更モードに入る/出る
            selected = !selected;
            break;
        }
        case MenuType::NONE:
        default:
            break;
    }
}

void MenuManager::changeValue(uint8_t selectedIndex, int newValue) {
    if (selectedIndex >= currentMenu.itemCount) {
        return; // 無効なインデックス
    }

    MenuItem selectedItem = currentMenu.items[selectedIndex];

    if (selectedItem.type == MenuType::VALUE_CHANGE) {
        // パラメータIDを取得して新しい値を設定
        ParamID paramID = selectedItem.actionParam.paramID;
        setParamValue(paramID, newValue);
    }
}

ParamConfig MenuManager::getCurrentParamConfig() const {
    return *getParamConfig(currentMenu.items[param].actionParam.paramID);
}