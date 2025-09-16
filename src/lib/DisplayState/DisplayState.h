#ifndef DisplayState_h
#define DisplayState_h

#include <Arduino.h>
#include <vector>

#include "MenuConfig.h"

enum MenuControlID {
    NEXT = 0,
    BACK,
    CHANGEVALUE
};

struct MenuItem {
    MenuIDs MenuID;
    std::vector<String> DisplayStrings;
    std::vector<MenuControlID> ControlID;
    std::vector<MenuIDs> CallMenuIDs;
    std::vector<int> OptionValues;
};

class DisplayState {
public:
    DisplayState(std::vector<MenuItem> MenuItems);
    ~DisplayState();

    void cusorUp();
    void cusorDown();
    int select();
    MenuIDs getCurrentMenuID();

private:
    std::vector<MenuItem> _menuItems;
    MenuIDs _currentMenuID;
    int _currentSelectionIndex;
};

#endif