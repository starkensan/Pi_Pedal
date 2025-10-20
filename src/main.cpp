#include <Arduino.h>

#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_MCP3421.h>
#include <Adafruit_NeoPixel.h>
#include <Adafruit_TinyUSB.h>
#include <MIDI.h>

#include "config.h"

#include "lib/driver/RotaryEncoder/RotaryEncoder.h"
#include "driver/PedalMon/PedalMon.h"
#include "driver/ExpPedal/ExpPedal.h"
#include "driver/DrawMenu/DrawMenu.h"

Adafruit_NeoPixel pixels(1, WS2812_PIN, NEO_GRB + NEO_KHZ800);

RotaryEncoder myRE(DT_PIN, CLK_PIN, RE_SW_PIN, 1000, 100);

Adafruit_USBD_MIDI usb_midi;

MIDI_CREATE_INSTANCE(Adafruit_USBD_MIDI, usb_midi, MIDI);

ExpPedal expPedal(&MIDI, 0);

PedalMon myPedals(&MIDI, PEDAL_PIN, PEDAL_CON_NUM, PEDAL_NUMBER);

DrawMenu myMenu(&Wire, SDA0_PIN, SCL0_PIN);

enum MenuState{
  DEDFALUT,
  MAIN_MENU,
  ASSIGN_MENU,
  PEDAL_MENU,
  PC_MENU,
  CC_MENU,
  EXPCC_MENU,
  MENU_COUNT
};

const int menuItemCount[MENU_COUNT]{
  0,
  3,
  8,
  3,
  3,
  4,
  3
};

MenuState currentMenuState = MAIN_MENU;

int currentSelection = 0;

int _test_value = 0;


/*
Callback Functions
*/

void onEncorderChange(){

  if(myRE.getDirection() > 0){
    currentSelection++;
  }else if(myRE.getDirection() < 0){
    currentSelection--;
  }

  if(currentSelection < 0){
    currentSelection = 0;
  }else if(currentSelection > menuItemCount[currentMenuState] - 1){
    currentSelection = menuItemCount[currentMenuState] - 1;
  }

  switch (currentMenuState) {
    case MAIN_MENU:
      myMenu.drawMainMenu(currentSelection);
      break;

    case ASSIGN_MENU:
      myMenu.drawAssignMenu(currentSelection);
      break;
  }
  
}

void onSwitchChange(){
  switch (currentMenuState) {
    case MAIN_MENU:
      if(currentSelection == 0){

      }else if(currentSelection == 1){
        currentMenuState = ASSIGN_MENU;
        myMenu.drawAssignMenu(currentSelection);
        currentSelection = 0;
      }else if(currentSelection == 2){
        
      } 
      break;
  }
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