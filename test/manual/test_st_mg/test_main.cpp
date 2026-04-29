#include <Arduino.h>
#include <unity.h>
#include <EepromStorage.hpp>
#include <SettingsManager/SettingsManager.hpp>
#include <config.h>

EepromStorage es;
SettingsManager *g_sm = nullptr;

// ------- ユーティリティ --------

// ------- テスト本体 --------

void test_begin() {
    g_sm->begin();
    TEST_PASS_MESSAGE("EepromStrage begin() executed without error");
}

void test_loadDefault() {
    g_sm->loadFactoryDefaults();
    TEST_ASSERT_EQUAL(g_sm->getPedalSettings(0).midiChannel, 1);
    TEST_ASSERT_EQUAL(g_sm->getPedalSettings(0).pedalMode, SettingsManager::PedalMode::CC);
    TEST_ASSERT_EQUAL(g_sm->getPedalSettings(0).switchBehavior, SettingsManager::SwitchBehavior::MOMENTARY);
    TEST_ASSERT_EQUAL(g_sm->getPedalSettings(1).midiChannel, 1);
    TEST_ASSERT_EQUAL(g_sm->getPedalSettings(1).pedalMode, SettingsManager::PedalMode::CC);
    TEST_ASSERT_EQUAL(g_sm->getPedalSettings(1).switchBehavior, SettingsManager::SwitchBehavior::MOMENTARY);
    for(int i=0; i<MAX_PEDALS; i++){
        TEST_ASSERT_EQUAL(g_sm->getPedalSettings(i).ccNumber, i+1);
    }
    
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
    SettingsManager sm(es);
    g_sm = &sm;
    
    // ---- テスト実行 ----
    RUN_TEST(test_begin);
    RUN_TEST(test_loadDefault);
    
    UNITY_END();
}

void loop() {
  // 何もしない（テストは setup() 内で完結）
}
