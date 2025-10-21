// test/test_display/test_main.cpp
#include <Arduino.h>
#include <unity.h>
#include <Pedal.h>
#include <config.h>

Pedal *g_pedal = nullptr;

// ------- ユーティリティ --------
bool stateChanged = false;
bool expectedState = false;

void callback(bool state){
    TEST_MESSAGE("Pedal state changed: ");
    TEST_MESSAGE(state ? "Released" : "Pressed");
    if (state != expectedState) {
        stateChanged = true;
        expectedState = state;
    }
}

// ------- テスト本体 --------

void test_pin_number() {
    TEST_ASSERT_EQUAL(PEDAL6_PIN, g_pedal->getPin());
}

void test_push_pedal_once() {
    // ペダルを一度押す動作をシミュレート
    TEST_MESSAGE("Please press and release the pedal once.");

    g_pedal->attachCallback(callback);
    unsigned long startTime = millis();
    while (millis() - startTime < 10000) { // 最大10秒待つ
        g_pedal->update();
        if (stateChanged && expectedState) { // 押して離したら終了
            TEST_PASS_MESSAGE("press pedal test completed without error");
            break;
        }
    }

    TEST_FAIL_MESSAGE("Pedal was not pressed and released within the time limit.");
}

void test_push_pedal_multiple_times() {
    int pressCount = 0;
    // ペダルを3回押す動作をシミュレート
    TEST_MESSAGE("Please press and release the pedal 3 times.");

    g_pedal->attachCallback(callback);
    unsigned long startTime = millis();
    while (millis() - startTime < 10000) { // 最大10秒待つ
        g_pedal->update();
        if (stateChanged && expectedState) { // 押して離したら終了
            stateChanged = false; // フラグリセット
            pressCount++;
            if (pressCount >= 3) {
                TEST_PASS_MESSAGE("press pedal multiple times test completed without error");
                break;
            }
        }
    }

    TEST_FAIL_MESSAGE("Pedal was not pressed and released within the time limit.");
}

// 必要なら setUp/tearDown で毎テストの前後処理を入れられます
void setUp() {
    stateChanged = false;
    expectedState = false;
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
    static Pedal pedal(PEDAL6_PIN);
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
