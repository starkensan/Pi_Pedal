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

    void render(int cursorIndesx, bool selected, MenuConfig currentMenu);
private:
    HalDisplay& display_;

    int lastHeadIndex = 0;
};

#endif // MENU_DISPLAY_HPP