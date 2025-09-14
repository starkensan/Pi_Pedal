#ifndef CONFIG_H
#define CONFIG_H

/*
Pin Configuration
*/

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

const int PEDAL_PIN[] = {PEDAL1_PIN, PEDAL2_PIN, PEDAL3_PIN, PEDAL4_PIN, PEDAL5_PIN, PEDAL6_PIN};

/*
Display Configuration
*/

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

#define FONT_WIDTH 8
#define FONT_HEIGHT 6

#define OLED_RESET     -1
#define SCREEN_ADDRESS 0x3C

/*
Other Configuration
*/

#define PEDAL_NUMBER 6

#define EXP_CH 0

const int PEDAL_CON_NUM[] = {1,2,3,4,5,6};

#endif