#ifndef DrawDisplay_h
#define DrawDisplay_h

#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <config.h>

#include <HalDisplay.hpp>

#define DEFAULT_SCREEN_ADDRESS 0x3C
#define OLED_RESET -1

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

#define FONT_WIDTH 8
#define FONT_HEIGHT 6
class DrawDisplay: public HalDisplay {
public:
    DrawDisplay(TwoWire* wireInstance, int SDA_PIN, int SCL_PIN, int width=SCREEN_WIDTH, int height=SCREEN_WIDTH, int address=DEFAULT_SCREEN_ADDRESS);
    ~DrawDisplay(){ delete display; }

    void begin() override;
    void clearDisplay() override;
    void drawCentreString(const String &buf) override;
    void drawCentreNumber(const int Number) override;
    void drawMenu(const String items[DRAW_MENU_MAX_ITEMS], int cursorIndex, bool invertCursor, const String rightTexts[3]) override;

private:
    Adafruit_SSD1306* display;
    TwoWire *wire;
    int SDA_PIN;
    int SCL_PIN;
    int ADDRESS;
    int screenWidth, screenHeight;

};

#endif