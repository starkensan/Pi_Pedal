#include <Arduino.h>

#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_MCP3421.h>
#include <Adafruit_NeoPixel.h>
#include <Adafruit_TinyUSB.h>
#include <MIDI.h>

#include "config.h"

#include "driver/RotaryEncoder/RotaryEncoder.h"
#include "driver/PedalMon/PedalMon.h"
#include "driver/ExpPedal/ExpPedal.h"
#include "driver/DrawMenu/DrawMenu.h"
#include "lib/DisplayState/DisplayState.h"

Adafruit_NeoPixel pixels(1, WS2812_PIN, NEO_GRB + NEO_KHZ800);

RotaryEncoder myRE(DT_PIN, CLK_PIN, RE_SW_PIN, 1000, 100);
Adafruit_USBD_MIDI usb_midi;
MIDI_CREATE_INSTANCE(Adafruit_USBD_MIDI, usb_midi, MIDI);


ExpPedal expPedal(&MIDI, 0);

PedalMon myPedals(&MIDI, PEDAL_PIN, PEDAL_CON_NUM, PEDAL_NUMBER);

DrawMenu myMenu(&Wire, SDA0_PIN, SCL0_PIN);

/*
Callback Functions
*/

void onEncorderChange(){
  
}

void onSwitchChange(){

}

void setup() {
  //Serial.begin(115200);

  pixels.begin();

  myMenu.init(SCREEN_WIDTH, SCREEN_HEIGHT);

  myRE.attachRotaryCallback(onEncorderChange);
  myRE.attachSwitchCallback(onSwitchChange);

  pixels.clear();
  pixels.setPixelColor(0, pixels.Color(128, 0, 0));
  pixels.show();
}

void setup1(){

  expPedal.begin(&Wire1, SDA1_PIN, SCL1_PIN);
  myPedals.begin();
  usb_midi.setStringDescriptor("PiPedal");
  MIDI.begin(1);
  while( !TinyUSBDevice.mounted() ) delay(1);
}

void loop() {
  myRE.checkEncoder();
  myRE.checkSwitch();
}

void loop1(){
  myPedals.update();
  expPedal.update();
}