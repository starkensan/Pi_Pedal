#ifndef MENU_DISPLAY_HPP
#define MENU_DISPLAY_HPP

#include <HalDisplay.hpp>
#include <MenuManager/MenuManager.hpp>
#include <MenuManager/MenuState.hpp>
#include <config.h>

using namespace MenuState;

class MenuDisplay {
public:
    MenuDisplay(HalDisplay& display, MenuManager& menuManager)
        : display_(display), menuManager_(menuManager) {}

    void render(int cursorIndesx);
private:
    HalDisplay& display_;
    MenuManager& menuManager_;

    int lastHeadIndex = 0;
};

#endif // MENU_DISPLAY_HPP