#include <Arduino.h>
#include <unity.h>
#include <DrawDisplay.h>

#include <Config.h>

// テスト対象
static DrawDisplay* g_dd = nullptr;

static bool oled_missing() {
  Wire.begin();
  Wire.setSDA(I2C_SDA_PIN);
  Wire.setSCL(I2C_SCL_PIN);
  delay(5);

  Wire.beginTransmission(SCREEN_ADDRESS);
  uint8_t err = Wire.endTransmission();
  return (err != 0);  // 0 以外なら見つからない
}