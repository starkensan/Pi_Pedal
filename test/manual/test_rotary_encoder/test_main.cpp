// test/test_display/test_main.cpp
#include <Arduino.h>
#include <unity.h>
#include <RotaryEncoder.hpp>
#include <config.h>

static RotaryEncoder *g_re = nullptr;

// ------- ユーティリティ --------
bool reChanged = false;
int expectedDirection = 0;
bool swChanged = false;


void reCallback(int direction, int count){
    reChanged = true;
    expectedDirection = direction;
}

void swCallback(){
    swChanged = true;
}

// ------- テスト本体 --------

void test_rotary_encoder_check_clockwise() {
    TEST_MESSAGE("Please rotate the encoder clockwise.");

    g_re->attachRotaryCallback(reCallback);

    unsigned long startTime = millis();
    while (millis() - startTime < 10000) { // 最大10秒待つ
        g_re->checkEncoder();
        if (reChanged && expectedDirection == 1) { // 時計回りに回したら
            TEST_PASS_MESSAGE("rotary encoder clockwise test completed without error");
            reChanged = false; // フラグリセット
        }
    }
    TEST_FAIL_MESSAGE("Rotary encoder was not turned clockwise within the time limit.");
}

void test_rotary_encoder_check_counterclockwise() {
    TEST_MESSAGE("Please rotate the encoder counter-clockwise.");

    g_re->attachRotaryCallback(reCallback);

    unsigned long startTime = millis();
    while (millis() - startTime < 10000) { // 最大10秒待つ
        g_re->checkEncoder();
        if (reChanged && expectedDirection == -1) { // 反時計回りに回したら
            TEST_PASS_MESSAGE("rotary encoder counter-clockwise test completed without error");
            return;
        }
    }
    TEST_FAIL_MESSAGE("Rotary encoder was not turned counter-clockwise within the time limit.");
}

void test_switch_check() {
    TEST_MESSAGE("Please press the encoder switch.");

    g_re->attachSwitchCallback(swCallback);

    unsigned long startTime = millis();
    while (millis() - startTime < 10000) { // 最大10秒待つ
        g_re->checkSwitch();
        if (swChanged) { // スイッチが押されたら
            TEST_PASS_MESSAGE("switch press test completed without error");
            return;
        }
    }
    TEST_FAIL_MESSAGE("Encoder switch was not pressed within the time limit.");
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
    g_re = &re;

    // ---- テスト実行 ----
    //RUN_TEST(test_rotary_encoder_check_clockwise);
    //RUN_TEST(test_rotary_encoder_check_counterclockwise);
    //RUN_TEST(test_switch_check);

    UNITY_END();
}

void loop() {
  // 何もしない（テストは setup() 内で完結）
}
