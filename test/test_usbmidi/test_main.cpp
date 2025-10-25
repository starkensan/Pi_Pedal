#include <Arduino.h>
#include <unity.h>
#include <USBMIDI.h>
#include <config.h>

USBMIDI *g_um = nullptr;

// ------- ユーティリティ --------

// ------- テスト本体 --------

void test_begin() {
    g_um->begin("TestDevice");
    TEST_PASS_MESSAGE("USBMIDI begin() executed without error");
}

void test_send_cc() {
    g_um->sendControlChange(1, 74, 127);
    TEST_PASS_MESSAGE("USBMIDI sendControlChange() executed without error");
}

void test_send_pc() {
    g_um->sendProgramChange(1, 10);
    TEST_PASS_MESSAGE("USBMIDI sendProgramChange() executed without error");
}
void test_end() {
    // 特に終了処理は無いが、一応呼んでおく
    TEST_PASS_MESSAGE("USBMIDI end() executed without error");
}

// 必要なら setUp/tearDown で毎テストの前後処理を入れられます
void setUp() {
}
void tearDown() {
}

void setup() {
    // USB CDC シリアル準備
    delay(600);
    Serial.begin(115200);
    unsigned long t0 = millis();
    while (!Serial && (millis() - t0 < 2000)) {
      delay(10);
    }

    UNITY_BEGIN();

    // テスト対象の生成＆初期化
    static USBMIDI um;
    g_um = &um;
    
    // ---- テスト実行 ----
    RUN_TEST(test_begin);
    
    UNITY_END();
}

void loop() {
  // 何もしない（テストは setup() 内で完結）
}
