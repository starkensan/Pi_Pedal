#include "MenuDisplay.hpp"

void MenuDisplay::begin() {
    display_.begin();
}

void MenuDisplay::render(int cursorIndex, bool selected, MenuConfig currentMenu) {

    if( currentMenu.itemCount <= cursorIndex ) {
        cursorIndex = currentMenu.itemCount - 1;
    }else if( cursorIndex < 0 ) {
        cursorIndex = 0;
    }

    if (prevMenuID != currentMenu.menuID) {
        // メニューが切り替わった場合、ヘッドインデックスをリセット
        lastHeadIndex = 0;
        prevMenuID = currentMenu.menuID;
    }

    if( lastHeadIndex > cursorIndex ) {
        lastHeadIndex = cursorIndex;
    }else if( lastHeadIndex + DRAW_MENU_MAX_ITEMS <= cursorIndex ) {
        lastHeadIndex = cursorIndex - DRAW_MENU_MAX_ITEMS + 1;
    }

    String items[DRAW_MENU_MAX_ITEMS];

    for(int i=0; i<DRAW_MENU_MAX_ITEMS; i++) {
        int itemIndex = lastHeadIndex + i;
        if(itemIndex < currentMenu.itemCount) {
            items[i] = String(currentMenu.items[itemIndex].label);
        } else {
            items[i] = String("");
        }
    }

    String rightTexts[DRAW_MENU_MAX_ITEMS];

    for(int i=0; i<DRAW_MENU_MAX_ITEMS; i++) {
        int itemIndex = lastHeadIndex + i;
        if(itemIndex < currentMenu.itemCount) {
            MenuItem menuItem = currentMenu.items[itemIndex];
            const ParamConfig& paramConfig = *getParamConfig(menuItem.actionParam.paramID);
            if(menuItem.type == MenuType::VALUE_CHANGE) {
                // パラメータの現在値を右側に表示
                if (paramConfig.isLabelMode) {
                    rightTexts[i] = String(paramConfig.labels[getParamValue(menuItem.actionParam.paramID)]);
                } else {
                    rightTexts[i] = String(getParamValue(menuItem.actionParam.paramID));
                }
            } else {
                rightTexts[i] = String("");
            }
        } else {
            rightTexts[i] = String("");
        }
    }

    int displayCursolIndex = cursorIndex - lastHeadIndex;

    display_.clearDisplay();
    display_.drawMenu(items, displayCursolIndex, selected, rightTexts);
}