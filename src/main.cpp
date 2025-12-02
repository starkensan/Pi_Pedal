#include <Arduino.h>
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

PedalsController pedals(pedal, expPedal, usbmidi, storage);

void setup() {
    pedals.begin("PiPedals");
    pedals.start();
}

void loop() {
    pedals.update();
}