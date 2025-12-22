#ifndef CONFIG_H
#define CONFIG_H

#define MCP_I2C_INSTANCE Wire1

#define OLED_I2C_INSTANCE Wire

#define PEDAL_COUNT 6
#define EXP_PEDAL_COUNT 1
#define MAX_PEDALS (PEDAL_COUNT + EXP_PEDAL_COUNT)

#define DRAW_MENU_MAX_ITEMS 3

#define EXP_CH 0

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

#endif