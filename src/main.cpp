#include <Arduino.h>
#include <config.h>

#include <Pedal.hpp>
#include <ExpPedal.hpp>
#include <USBMIDI.hpp>

#include <DrawDisplay.hpp>
#include <RotaryEncoder.hpp>

#include <EepromStorage.hpp>

#include <SettingsManager/SettingsManager.hpp>

#include <PedalsController/PedalsController.hpp>

#include <MenuController/MenuController.hpp>

Pedal pedal;
ExpPedal expPedal;
USBMIDI usbmidi;
EepromStorage storage;

DrawDisplay display(&OLED_I2C_INSTANCE, SDA0_PIN, SCL0_PIN);
RotaryEncoder rotaryEncoder(DT_PIN, CLK_PIN, RE_SW_PIN);

SettingsManager settings(storage);

PedalsController pedals(pedal, expPedal, usbmidi, settings);
MenuController menuController(display, rotaryEncoder, settings);

mutex_t mtx_;

struct LockGuard {
    mutex_t& m;
    LockGuard(mutex_t& m_) : m(m_) { mutex_enter_blocking(&m); }
    ~LockGuard() { mutex_exit(&m); }
};

int PCCurrentNumber_ = 0;
bool PC_flag = false;

void pcNumberCallback(int pcNumber){
    PCCurrentNumber_ = pcNumber;
    PC_flag = true;
}

void setup(){
    settings.begin();
    pedals.attachPCCallbacks(pcNumberCallback);
    pedals.begin("PiPedals");
    pedals.start();
}

void loop(){
    pedals.getPCCurrentNumber();
    pedals.update();
}

void setup1() {
    delay(3000);
    settings.begin();
    menuController.begin();
}

void loop1() {
    if(PC_flag) {
        menuController.showPCNumber(PCCurrentNumber_);
        PC_flag = false;
    }
    menuController.update();
}