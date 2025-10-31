#include <Arduino.h>
#include <DebugLog.h>
#include <config.h>
#include <PedalsController/PedalsController.hpp>
#include <Pedal.hpp>
#include <ExpPedal.hpp>
#include <USBMIDI.hpp>
#include <EepromStorage.hpp>

Pedal pedal;
ExpPedal expPedal;
USBMIDI usbmidi;
EepromStorage storage;

PedalsController pedals(&pedal, expPedal, usbmidi, storage);

void setup() {

    LOG_SET_LEVEL(DebugLogLevel::LVL_INFO);
    //Serialが立ち上がるまで待つ
    Serial.begin(115200);

    Serial.println("Start serial");

    delay(1000);

    pedals.begin("PiPedals");

    delay(1000);
    
}

void loop() {
    pedals.update();
}