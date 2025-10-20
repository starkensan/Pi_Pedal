#ifndef DrawDisplay_h
#define DrawDisplay_h

#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#include <Config.h>

class DrawDisplay {
public:
    DrawDisplay(TwoWire* wireInstance, int SDA, int SCL);

    void init(int screenWidth, int screenHeight);
    void drawCentreString(const String &buf);
    void drawCentreNumber(const int Number);
    void drawMenu(const String items[3], int cursorIndex, bool invertCursor, const String rightTexts[3]);

private:
    Adafruit_SSD1306* display;
    TwoWire *wire;
    int ADDRESS;
};

#endif