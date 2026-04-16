#ifndef MENU_CONTROLLER_HPP
#define MENU_CONTROLLER_HPP

#include <MenuManager/MenuManager.hpp>
#include <MenuManager/MenuState.hpp>
#include <HalRotaryEncoder.hpp>
#include <HalDisplay.hpp>
#include <SettingsManager/SettingsManager.hpp>

using namespace MenuState;

class MenuController {
public:
    MenuController(HalDisplay& display, HalRotaryEncoder& rotaryEncoder, SettingsManager& settingsManager)
        : rotaryEncoder_(rotaryEncoder),  menuManager_(settingsManager, display){
        self = this;
    }
    ~MenuController() = default;

    void begin();

    void update();

    void showPCNumber(int number){ menuManager_.showPCNumber(number); }

private:
    static MenuController* self;

    static void staticRotaryCallback(int direction, int count);
    static void staticSwitchCallback();

    void RotaryCallback(int direction, int count);
    void SwitchCallback();

    MenuManager menuManager_;

    HalRotaryEncoder& rotaryEncoder_;
};

#endif // MENU_CONTROLLER_HPP