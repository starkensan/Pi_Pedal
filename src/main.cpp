#include <Arduino.h>
#include <Adafruit_TinyUSB.h>
#include <config.h>

#include <DrawDisplay.hpp>

DrawDisplay display(&OLED_I2C_INSTANCE, SDA0_PIN, SCL0_PIN);

void setup() {
    display.begin();
}
void loop() {
    display.drawMenu(
        (String[DRAW_MENU_MAX_ITEMS]){"Item 1", "Item 2", "Item 3"},
        1,
        false,
        (String[3]){"R1", "R2", "R3"}
    );
}