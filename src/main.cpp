#include <Arduino.h>
#include <Adafruit_TinyUSB.h>
#include <config.h>

#include <Pedal.hpp>
#include <ExpPedal.hpp>
#include <USBMIDI.hpp>
#include <EepromStorage.hpp>

#include <SettingsManager/SettingsManager.hpp>
#include <PedalsController/PedalsController.hpp>

Pedal pedal;
ExpPedal expPedal;
USBMIDI usbmidi;
EepromStorage storage;

SettingsManager settings(storage);

PedalsController pedals(pedal, expPedal, usbmidi, settings);

void setup() {
    pedals.begin("PiPedals");
    pedals.start();
}

void loop() {
    pedals.update();
}