#include <Arduino.h>

#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_MCP3421.h>
#include <Adafruit_NeoPixel.h>
#include <Adafruit_TinyUSB.h>
#include <MIDI.h>

#include "RotaryEncoder.h"
#include "PedalMonitor.h"
#include "ExpressionPedal.h"

#define EXP_PIN 28

#define CLK_PIN 15
#define DT_PIN 26
#define RE_SW_PIN 27

#define SCL1_PIN 3
#define SDA1_PIN 2

#define SCL0_PIN 5
#define SDA0_PIN 4

#define PEDAL1_PIN 13
#define PEDAL2_PIN 14
#define PEDAL3_PIN 10
#define PEDAL4_PIN 9
#define PEDAL5_PIN 12
#define PEDAL6_PIN 11

#define WS2812_PIN 16

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

#define FONT_WIDTH 8
#define FONT_HEIGHT 6

#define OLED_RESET     -1
#define SCREEN_ADDRESS 0x3C

#define PEDAL_NUMBER 6

#define EXP_CH 0

const int PEDAL_PIN[] = {PEDAL1_PIN, PEDAL2_PIN, PEDAL3_PIN, PEDAL4_PIN, PEDAL5_PIN, PEDAL6_PIN};

int PedalControlNumber[] = {1,2,3,4,5,6};

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

Adafruit_NeoPixel pixels(1, WS2812_PIN, NEO_GRB + NEO_KHZ800);

RotaryEncoder myRE(DT_PIN, CLK_PIN, RE_SW_PIN, 1000, 100);

Adafruit_USBD_MIDI usb_midi;

MIDI_CREATE_INSTANCE(Adafruit_USBD_MIDI, usb_midi, MIDI);

ExpressionPedal expPedal(&MIDI, 0);

PedalMonitor myPedals(&MIDI, PEDAL_PIN, PedalControlNumber, PEDAL_NUMBER);

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
Menu Drwaing Functions
*/
void display_init(){
  Wire.setSDA(SDA0_PIN);
  Wire.setSCL(SCL0_PIN);

  if(!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Don't proceed, loop forever
  }

  display.display();

  delay(2000);

  display.clearDisplay();
  delay(1000);

}

void drawCentreString(const String &buf)
{
  int16_t x1, y1;
  uint16_t w, h;
  display.getTextBounds(buf, SCREEN_WIDTH/2, SCREEN_HEIGHT/2, &x1, &y1, &w, &h);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(SCREEN_WIDTH/2 - w / 2, SCREEN_HEIGHT/2 - h / 2);
  display.print(buf);
}

void drawCentreNumber(const int Number){

  if(Number >= 100 || Number <= -10){
    display.setTextSize(3);
  }else{
    display.setTextSize(4);
  }
  drawCentreString(String(Number));
}

void drawMainMenu(int select){
  const String MenuString[3] = {"Exit", "Assign", "Setting"};
  display.clearDisplay();
  display.setTextSize(2);
  display.setTextColor(SSD1306_WHITE);


  for(int i=0; i<3; i++){
    display.setCursor(FONT_WIDTH*2, (FONT_HEIGHT*3 + 4)*i);
    display.print(MenuString[i]);
  }


  display.setCursor(0, (FONT_HEIGHT*3 + 4)*select);
  display.print(">");

  display.display();

}


void drawAssignMenu(int select){

  const String AssignMenuString[3] = {"Exit", "Pedal", "Pedal EXP"};
  static int displayRenge = 0;

  if(select >= 3 && select >= displayRenge+2){
    displayRenge = select - 2;
  }else if(select < displayRenge){
    displayRenge = select;
  }
  
  display.clearDisplay();
  display.setTextSize(2);
  display.setTextColor(SSD1306_WHITE);


  for(int i=0; i<3; i++){
    display.setCursor(FONT_WIDTH*2, (FONT_HEIGHT*3 + 4)*i);
    
    if(i+displayRenge == 0){
      display.print(AssignMenuString[0]);
    }else if(i+displayRenge == PEDAL_NUMBER + 1){
      display.print(AssignMenuString[2]);
    }else if(i+displayRenge < PEDAL_NUMBER+1){
      display.print(AssignMenuString[1] + " " + String(i+displayRenge));
    }
    
  }

  display.setCursor(0, (FONT_HEIGHT*3 + 4)*(select-displayRenge));
  display.print(">");

  display.display();
  Serial.println(select);

}

void drawPedalMenu(int select){

  const String PedalMenuString[3] = {"Exit", "PC", "CC"};
  display.clearDisplay();
  display.setTextSize(2);
  display.setTextColor(SSD1306_WHITE);


  for(int i=0; i<3; i++){
    display.setCursor(FONT_WIDTH*2, (FONT_HEIGHT*3 + 4)*i);
    display.print(PedalMenuString[i]);
  }


  display.setCursor(0, (FONT_HEIGHT*3 + 4)*select);
  display.print(">");

  display.display();

}

void drawPCMenu(int select, int Num, int Ch, bool invert){
  const String PCMenuString[3] = {"Exit", "No:", "CH:"};
  int Data[2] = {Num, Ch};
  
  display.clearDisplay();
  display.setTextSize(2);
  display.setTextColor(SSD1306_WHITE);


  for(int i=0; i<3; i++){
    display.setCursor(FONT_WIDTH*2, (FONT_HEIGHT*3 + 4)*i);
    if(i>0){
      display.print(PCMenuString[i]);
      if(invert && select == i)display.setTextColor(SSD1306_BLACK, SSD1306_WHITE);
      display.print(String(Data[i]));
      if(invert && select == i)display.setTextColor(SSD1306_WHITE);
    }else{
      display.print(PCMenuString[i]);
    }
  }
  display.setCursor(0, (FONT_HEIGHT*3 + 4)*select);
  display.print(">");

  display.display();

}

void drawCCMenu(int select, int value, int Num, int Ch, bool invert){

  const String CCMenuString[4] = {"Exit", "Value:", "No:", "CH:"};
  int Data[3] ={value, Num, Ch};
  static int displayRenge = 0;

  if(select >= 3 && select >= displayRenge+2){
    displayRenge = select - 2;
  }else if(select < displayRenge){
    displayRenge = select;
  }
  
  display.clearDisplay();
  display.setTextSize(2);
  display.setTextColor(SSD1306_WHITE);


  for(int i=0; i<3; i++){
    display.setCursor(FONT_WIDTH*2, (FONT_HEIGHT*3 + 4)*i);
    display.print(CCMenuString[i+displayRenge]);
    if(displayRenge + i > 0){
      if(invert && select == i + displayRenge)display.setTextColor(SSD1306_BLACK, SSD1306_WHITE);
      display.print(String(Data[i + displayRenge]));
      if(invert && select == i + displayRenge)display.setTextColor(SSD1306_WHITE);
    }
  }

  display.setCursor(0, (FONT_HEIGHT*3 + 4)*(select-displayRenge));
  display.print(">");
  

  display.display();

}

void drawExpCCMenu(int select, int Num, int Ch, bool invert){

  const String ExpCCMenuString[3] = {"Exit", "No:", "CH:"};
  int Data[2] = {Num, Ch};
  
  display.clearDisplay();
  display.setTextSize(2);
  display.setTextColor(SSD1306_WHITE);


  for(int i=0; i<3; i++){
    display.setCursor(FONT_WIDTH*2, (FONT_HEIGHT*3 + 4)*i);
    if(i>0){
      display.print(ExpCCMenuString[i]);
      if(invert && select == i)display.setTextColor(SSD1306_BLACK, SSD1306_WHITE);
      display.print(String(Data[i]));
      if(invert && select == i)display.setTextColor(SSD1306_WHITE);
    }else{
      display.print(ExpCCMenuString[i]);
    }
  }
  display.setCursor(0, (FONT_HEIGHT*3 + 4)*select);
  display.print(">");

  display.display();

}


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
      drawMainMenu(currentSelection);
      break;

    case ASSIGN_MENU:
      drawAssignMenu(currentSelection);
      break;
  }
  
}

void onSwitchChange(){
  switch (currentMenuState) {
    case MAIN_MENU:
      if(currentSelection == 0){

      }else if(currentSelection == 1){
        currentMenuState = ASSIGN_MENU;
        drawAssignMenu(currentSelection);
        currentSelection = 0;
      }else if(currentSelection == 2){
        
      } 
      break;
  }
}

void setup() {
  //Serial.begin(115200);

  pixels.begin();

  display_init();

  myRE.attachRotaryCallback(onEncorderChange);
  myRE.attachSwitchCallback(onSwitchChange);

  pixels.clear();
  pixels.setPixelColor(0, pixels.Color(128, 0, 0));
  pixels.show();
}

void setup1(){

  expPedal.begin(&Wire1, SDA1_PIN, SCL1_PIN);

  myPedals.begin();

  usb_midi.setStringDescriptor("MyPedal");

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