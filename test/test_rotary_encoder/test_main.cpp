// test/test_display/test_main.cpp
#include <Arduino.h>
#include <unity.h>
#include <RotaryEncoder.h>
#include <config.h>

static RotaryEncoder *g_re = nullptr;

// ------- ユーティリティ --------
bool valueChanged = false;
bool expectedValue = false;

void callback(bool state){

}

// ------- テスト本体 --------

void test_pin_number() {
    TEST_ASSERT_EQUAL(PEDAL6_PIN, g_pedal->getPin());
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
        ; // タイムアウト付きで待つ（ヘッドレスでも先へ進む）
    }

    UNITY_BEGIN();

    // テスト対象の生成＆初期化
    static RotaryEncoder re(DT_PIN, CLK_PIN, RE_SW_PIN);
    g_pedal = &pedal;

    g_pedal->begin();
    g_pedal->attachCallback([](bool state){
        TEST_MESSAGE("Pedal state changed: ");
        TEST_MESSAGE(state ? "Released" : "Pressed");
    });

    // ---- テスト実行 ----
    RUN_TEST(test_pin_number);
    //RUN_TEST(test_push_pedal_once);
    //RUN_TEST(test_push_pedal_multiple_times);

    UNITY_END();
}

void loop() {
  // 何もしない（テストは setup() 内で完結）
}
