#ifndef DrawMenu_h
#define DrawMenu_h

#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#include "config.h"

class DrawMenu {
public:
    DrawMenu(TwoWire* wireInstance, int SDA, int SCL);

    void init(int screenWidth, int screenHeight);
    void drawCentreString(const String &buf);
    void drawCentreNumber(const int Number);
    void drawMainMenu(int select);
    void drawAssignMenu(int select);
    void drawPedalMenu(int select);
    void drawPCMenu(int select, int Num, int Value, bool invert);
    void drawCCMenu(int select, int value, int Num, int Ch, bool invert);
    void drawExpCCMenu(int select, int Num, int Ch, bool invert);

private:
    Adafruit_SSD1306* display;
    TwoWire *wire;
    int ADDRESS;
};

#endif