#include "MenuDisplay.hpp"
void MenuDisplay::render(int cursorIndex) {
    MenuConfig currentMenu = menuManager_.getCurrentMenu();

    if( currentMenu.itemCount <= cursorIndex ) {
        cursorIndex = currentMenu.itemCount - 1;
    }else if( cursorIndex < 0 ) {
        cursorIndex = 0;
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
                if (paramConfig.isLabelMode && 
                    paramConfig.value >= 0 && 
                    paramConfig.value <= paramConfig.maxValue) {
                    rightTexts[i] = String(paramConfig.labels[paramConfig.value]);
                } else {
                    rightTexts[i] = String(paramConfig.value);
                }
            } else {
                rightTexts[i] = String("");
            }
        } else {
            rightTexts[i] = String("");
        }
    }

    display_.clearDisplay();
    display_.drawMenu(items, cursorIndex, menuManager_.isSelected(), rightTexts);
}