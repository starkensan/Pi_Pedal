#ifndef MENU_DISPLAY_HPP
#define MENU_DISPLAY_HPP

#include <HalDisplay.hpp>
#include <MenuManager/MenuState.hpp>
#include <config.h>

using namespace MenuState;

class MenuDisplay {
public:
    MenuDisplay(HalDisplay& display)
        : display_(display){}
    
    void begin();

    void renderNumber(int number) {
        display_.drawCentreNumber(number);
    }
    void render(int cursorIndesx, bool selected, MenuConfig currentMenu);
    void clear() {
        display_.clearDisplay();
    }
private:
    HalDisplay& display_;

    MenuID prevMenuID = MenuID::MAIN;

    int lastHeadIndex = 0;
};

#endif // MENU_DISPLAY_HPP