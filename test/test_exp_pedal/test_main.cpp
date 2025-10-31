#include <Arduino.h>
#include <unity.h>
#include <ExpPedal.hpp>
#include <config.h>
#include <Wire.h>

// テスト対象
ExpPedal* g_ep = nullptr;

// ------- ユーティリティ --------

bool valueChanged = false;
int expectedValue = 0;

void callback(int value) {
    valueChanged = true;
    expectedValue = value;
}

// I2Cの疎通確認（デバイスが見つからなければスキップ用に true を返す）
static bool mcp_missing() {
    Wire1.setSDA(SDA1_PIN);
    Wire1.setSCL(SCL1_PIN);
    Wire1.begin();
    delay(5);

    Wire1.beginTransmission(DEFAULT_ADC_ADDRESS);
    uint8_t err = Wire1.endTransmission();
    return (err != 0);  // 0 以外なら見つからない
}

// ------- テスト本体 --------

void test_exp_pedal_instance() {
    TEST_ASSERT_NOT_NULL(g_ep);
}

void test_exp_pedal_min() {
    g_ep->attachCallback(callback);
    TEST_MESSAGE("Please move the expression pedal to minimum position.");
    unsigned long startTime = millis();
    while (millis() - startTime < 10000) { // 最大10秒待つ
        g_ep->update();
        if (valueChanged && expectedValue == 0) {
            valueChanged = false; // リセット
            TEST_PASS_MESSAGE("ExpPedal minimum position test completed without error");
            return;
        }
    }
    TEST_FAIL_MESSAGE("Expression pedal was not moved to minimum position within the time limit.");
}

void test_exp_pedal_max() {
    g_ep->attachCallback(callback);
    TEST_MESSAGE("Please move the expression pedal to maximum position.");
    unsigned long startTime = millis();
    while (millis() - startTime < 10000) { // 最大10秒待つ
        g_ep->update();
        if (expectedValue >= 120) { // 最大付近に達したらOKとする
            valueChanged = false; // リセット
            TEST_PASS_MESSAGE("ExpPedal maximum position test completed without error");
            return;
        }
    }
    TEST_FAIL_MESSAGE("Expression pedal was not moved to maximum position within the time limit.");
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

    // MCP が見つからなければスキップ
    if (mcp_missing()) {
        TEST_IGNORE_MESSAGE("ExpPedal ADC not found on I2C bus; skipping display tests.");
        UNITY_END();
        return;
    }

    // テスト対象の生成＆初期化
    static ExpPedal ep;
    g_ep = &ep;

    // 注意: init() 内で begin() 失敗時は for(;;) で止まる実装なので、
    // ここに来る時点で I2C 疎通が取れていることを事前に確認しています。
    g_ep->begin(&MCP_I2C_INSTANCE, SDA1_PIN, SCL1_PIN);

    // ---- テスト実行 ----
    RUN_TEST(test_exp_pedal_instance);
    RUN_TEST(test_exp_pedal_min);
    RUN_TEST(test_exp_pedal_max);

    UNITY_END();
}

void loop() {
  // 何もしない（テストは setup() 内で完結）
}
