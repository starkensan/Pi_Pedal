#ifndef MENU_CONTROLLER_HPP
#define MENU_CONTROLLER_HPP

#include <MenuManager/MenuManager.hpp>
#include <MenuManager/MenuState.hpp>
#include <MenuDisplay/MenuDisplay.hpp>
#include <HalRotaryEncoder.hpp>
#include <SettingsManager/SettingsManager.hpp>

using namespace MenuState;

class MenuController {
public:
    MenuController(MenuManager& menuManager, HalRotaryEncoder& rotaryEncoder, SettingsManager& settingsManager)
        : menuManager_(menuManager), rotaryEncoder_(rotaryEncoder), settingsManager_(settingsManager) {}
    ~MenuController() = default;

    void begin();

private:
    MenuManager& menuManager_;
    HalRotaryEncoder& rotaryEncoder_;
    SettingsManager& settingsManager_;
};

#endif // MENU_CONTROLLER_HPP