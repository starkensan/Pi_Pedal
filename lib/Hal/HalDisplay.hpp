#ifndef HALDISPLAY_HPP
#define HALDISPLAY_HPP

#include <Arduino.h>
#include <Wire.h>

#define DEFAULT_ADC_ADDRESS 0x68

class HalDisplay {
public:
    virtual ~HalDisplay() {}

    // OLED等の初期化処理
    virtual void begin() = 0;

    // 画面全体をクリア
    virtual void clearDisplay() = 0;

    // 中央に文字列を描画
    virtual void drawCentreString(const String& buf) = 0;

    // 中央に数値を描画
    virtual void drawCentreNumber(int number) = 0;

    // メニュー項目を描画
    virtual void drawMenu(const String items[3],
                          int cursorIndex,
                          bool invertCursor,
                          const String rightTexts[3]) = 0;
};

#endif
