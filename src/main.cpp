#include <Arduino.h>
#include <DebugLog.h>
#include <Adafruit_TinyUSB.h>
#include <config.h>

#include <Pedal.hpp>
#include <ExpPedal.hpp>
#include <USBMIDI.hpp>
#include <EepromStorage.hpp>

#include <PedalsController/PedalsController.hpp>

Pedal pedal;
ExpPedal expPedal;
USBMIDI usbmidi;
EepromStorage storage;

PedalsController pedals(&pedal, expPedal, usbmidi, storage);



void setup() {

    LOG_ATTACH_SERIAL(SerialTinyUSB);
    LOG_SET_LEVEL(DebugLogLevel::LVL_INFO);

    pedals.begin("PiPedals");

    delay(1000);
    
}

void loop() {
    pedals.update();
}