#include <Arduino.h>
#include <DebugLog.h>
#include <config.h>

void setup() {
    Serial.begin(115200);

    LOG_SET_LEVEL(DebugLogLevel::LVL_INFO);
}

void loop() {
}