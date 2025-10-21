#ifndef DrawDisplay_h
#define DrawDisplay_h

#include <Arduino.h>
#include <Wire.h>
#include <DebugLog.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define DEFAULT_SCREEN_ADDRESS 0x3C
#define OLED_RESET -1

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

#define FONT_WIDTH 8
#define FONT_HEIGHT 6
class DrawDisplay {
public:
    DrawDisplay(TwoWire* wireInstance, int SDA, int SCL);

    void begin(int screenWidth, int screenHeight, int screenAddress=DEFAULT_SCREEN_ADDRESS);
    void clearDisplay();
    void drawCentreString(const String &buf);
    void drawCentreNumber(const int Number);
    void drawMenu(const String items[3], int cursorIndex, bool invertCursor, const String rightTexts[3]);

private:
    Adafruit_SSD1306* display;
    TwoWire *wire;
    int ADDRESS;
    int screenWidth, screenHeight;

};

#endif