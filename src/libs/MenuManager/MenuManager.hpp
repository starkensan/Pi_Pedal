#ifndef MENU_MANAGER_HPP
#define MENU_MANAGER_HPP
#include "MenuState.hpp"

using namespace MenuState;

class MenuManager {
public:
    MenuManager() = default;
    ~MenuManager() = default;

    void init(MenuConfig& initMenu);
    
    MenuConfig getCurrentMenu() const;
    ParamConfig getCurrentParamConfig() const;

    void enterSelectedItem(uint8_t selectedIndex);
    void changeValue(uint8_t selectedIndex, int newValue);

private:

    MenuConfig currentMenu;
    bool selected;
    int param;
    
};
#endif // MENU_MANAGER_HPP