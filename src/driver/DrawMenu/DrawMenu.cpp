#include "DrawMenu.h"

DrawMenu::DrawMenu(TwoWire* wireInstance, int SDA_PIN, int SCL_PIN) : wire(wireInstance){
    Wire.setSDA(SDA_PIN);
    Wire.setSCL(SCL_PIN);
}

/*
Menu Drwaing Functions
*/
void DrawMenu::init(int screenWidth, int screenHeight){
    display = new Adafruit_SSD1306(screenWidth, screenHeight, wire, OLED_RESET);

  if(!display->begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Don't proceed, loop forever
  }

  display->display();

}

void DrawMenu::drawCentreString(const String &buf)
{
  int16_t x1, y1;
  uint16_t w, h;
  display->getTextBounds(buf, SCREEN_WIDTH/2, SCREEN_HEIGHT/2, &x1, &y1, &w, &h);
  display->setTextColor(SSD1306_WHITE);
  display->setCursor(SCREEN_WIDTH/2 - w / 2, SCREEN_HEIGHT/2 - h / 2);
  display->print(buf);
}

void DrawMenu::drawCentreNumber(const int Number){

  if(Number >= 100 || Number <= -10){
    display->setTextSize(3);
  }else{
    display->setTextSize(4);
  }
  drawCentreString(String(Number));
}

void DrawMenu::drawMainMenu(int select){
  const String MenuString[3] = {"Exit", "Assign", "Setting"};
  display->clearDisplay();
  display->setTextSize(2);
  display->setTextColor(SSD1306_WHITE);


  for(int i=0; i<3; i++){
    display->setCursor(FONT_WIDTH*2, (FONT_HEIGHT*3 + 4)*i);
    display->print(MenuString[i]);
  }


  display->setCursor(0, (FONT_HEIGHT*3 + 4)*select);
  display->print(">");

  display->display();

}


void DrawMenu::drawAssignMenu(int select){

  const String AssignMenuString[3] = {"Exit", "Pedal", "Pedal EXP"};
  static int displayRenge = 0;

  if(select >= 3 && select >= displayRenge+2){
    displayRenge = select - 2;
  }else if(select < displayRenge){
    displayRenge = select;
  }
  
  display->clearDisplay();
  display->setTextSize(2);
  display->setTextColor(SSD1306_WHITE);


  for(int i=0; i<3; i++){
    display->setCursor(FONT_WIDTH*2, (FONT_HEIGHT*3 + 4)*i);
    
    if(i+displayRenge == 0){
      display->print(AssignMenuString[0]);
    }else if(i+displayRenge == PEDAL_NUMBER + 1){
      display->print(AssignMenuString[2]);
    }else if(i+displayRenge < PEDAL_NUMBER+1){
      display->print(AssignMenuString[1] + " " + String(i+displayRenge));
    }
    
  }

  display->setCursor(0, (FONT_HEIGHT*3 + 4)*(select-displayRenge));
  display->print(">");

  display->display();

}

void DrawMenu::drawPedalMenu(int select){

  const String PedalMenuString[3] = {"Exit", "PC", "CC"};
  display->clearDisplay();
  display->setTextSize(2);
  display->setTextColor(SSD1306_WHITE);


  for(int i=0; i<3; i++){
    display->setCursor(FONT_WIDTH*2, (FONT_HEIGHT*3 + 4)*i);
    display->print(PedalMenuString[i]);
  }


  display->setCursor(0, (FONT_HEIGHT*3 + 4)*select);
  display->print(">");

  display->display();

}

void DrawMenu::drawPCMenu(int select, int Num, int Ch, bool invert){
  const String PCMenuString[3] = {"Exit", "No:", "CH:"};
  int Data[2] = {Num, Ch};
  
  display->clearDisplay();
  display->setTextSize(2);
  display->setTextColor(SSD1306_WHITE);


  for(int i=0; i<3; i++){
    display->setCursor(FONT_WIDTH*2, (FONT_HEIGHT*3 + 4)*i);
    if(i>0){
      display->print(PCMenuString[i]);
      if(invert && select == i)display->setTextColor(SSD1306_BLACK, SSD1306_WHITE);
      display->print(String(Data[i]));
      if(invert && select == i)display->setTextColor(SSD1306_WHITE);
    }else{
      display->print(PCMenuString[i]);
    }
  }
  display->setCursor(0, (FONT_HEIGHT*3 + 4)*select);
  display->print(">");

  display->display();

}

void DrawMenu::drawCCMenu(int select, int value, int Num, int Ch, bool invert){

  const String CCMenuString[4] = {"Exit", "Value:", "No:", "CH:"};
  int Data[3] ={value, Num, Ch};
  static int displayRenge = 0;

  if(select >= 3 && select >= displayRenge+2){
    displayRenge = select - 2;
  }else if(select < displayRenge){
    displayRenge = select;
  }
  
  display->clearDisplay();
  display->setTextSize(2);
  display->setTextColor(SSD1306_WHITE);


  for(int i=0; i<3; i++){
    display->setCursor(FONT_WIDTH*2, (FONT_HEIGHT*3 + 4)*i);
    display->print(CCMenuString[i+displayRenge]);
    if(displayRenge + i > 0){
      if(invert && select == i + displayRenge)display->setTextColor(SSD1306_BLACK, SSD1306_WHITE);
      display->print(String(Data[i + displayRenge]));
      if(invert && select == i + displayRenge)display->setTextColor(SSD1306_WHITE);
    }
  }

  display->setCursor(0, (FONT_HEIGHT*3 + 4)*(select-displayRenge));
  display->print(">");
  

  display->display();

}

void DrawMenu::drawExpCCMenu(int select, int Num, int Ch, bool invert){

  const String ExpCCMenuString[3] = {"Exit", "No:", "CH:"};
  int Data[2] = {Num, Ch};
  
  display->clearDisplay();
  display->setTextSize(2);
  display->setTextColor(SSD1306_WHITE);


  for(int i=0; i<3; i++){
    display->setCursor(FONT_WIDTH*2, (FONT_HEIGHT*3 + 4)*i);
    if(i>0){
      display->print(ExpCCMenuString[i]);
      if(invert && select == i)display->setTextColor(SSD1306_BLACK, SSD1306_WHITE);
      display->print(String(Data[i]));
      if(invert && select == i)display->setTextColor(SSD1306_WHITE);
    }else{
      display->print(ExpCCMenuString[i]);
    }
  }
  display->setCursor(0, (FONT_HEIGHT*3 + 4)*select);
  display->print(">");

  display->display();

}

