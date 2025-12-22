#include "MenuController.hpp"

MenuController* MenuController::self = nullptr;

void MenuController::begin() {
    rotaryEncoder_.attachRotaryCallback(staticRotaryCallback);
    rotaryEncoder_.attachSwitchCallback(staticSwitchCallback);
    menuManager_.init(MenuID::SETTINGS);
}

void MenuController::update() {
    rotaryEncoder_.checkEncoder();
    rotaryEncoder_.checkSwitch();
}

void MenuController::staticRotaryCallback(int direction, int count) {
    if (self) {
        self->RotaryCallback(direction, count);
    }
}

void MenuController::staticSwitchCallback() {
    if (self) {
        self->SwitchCallback();
    }
}

void MenuController::RotaryCallback(int direction, int count) {
    if(direction > 0) {
        menuManager_.cusorDown(count);
    } else if(direction < 0) {
        menuManager_.cusorUp(-count);
    }
}

void MenuController::SwitchCallback() {
    menuManager_.enterSelectedItem();
}