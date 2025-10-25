// test/unity_config.cpp
#include <Arduino.h>

extern "C" {

void unityOutputStart(void) {
    // 必要ならシリアル初期化など
    Serial.begin(115200);
}

void unityOutputChar(char c) {
    // 文字1つずつ吐く
    Serial.write(c);
}

void unityOutputFlush(void) {
    Serial.flush();
}

void unityOutputComplete(void) {
    // 終了時フック。何もしなくてOK
}

} // extern "C"
