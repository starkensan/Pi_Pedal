#include <Arduino.h>
#include <unity.h>
#include <EepromStrage.h>
#include <config.h>

EepromStrage *g_es = nullptr;

// ------- ユーティリティ --------

enum MidiMode
{
    MIDI_MODE_OFF = 0,
    MIDI_MODE_CC,
    MIDI_MODE_PC_NEXT,
    MIDI_MODE_PC_BACK
};

struct PedalMidiSetting
{
    uint8_t pedalNum;
    MidiMode midiMode;
    int ccNumber;
};

PedalMidiSetting setting = {1, MIDI_MODE_CC, 74};

// ------- テスト本体 --------

void test_begin() {
    g_es->begin(sizeof(setting));
    TEST_PASS_MESSAGE("EepromStrage begin() executed without error");
}

void test_put_get() {
    g_es->put(0, &setting, sizeof(PedalMidiSetting));
    TEST_ASSERT_TRUE(g_es->commit());
    
    PedalMidiSetting readSetting;
    g_es->get(0, &readSetting, sizeof(PedalMidiSetting));
    
    TEST_ASSERT_EQUAL_UINT8(setting.pedalNum, readSetting.pedalNum);
    TEST_ASSERT_EQUAL_UINT8(setting.midiMode, readSetting.midiMode);
    TEST_ASSERT_EQUAL_INT(setting.ccNumber, readSetting.ccNumber);
}
void test_end() {
    g_es->end();
    TEST_PASS_MESSAGE("EepromStrage end() executed without error");
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
    static EepromStrage es;
    g_es = &es;
    
    // ---- テスト実行 ----
    RUN_TEST(test_begin);
    RUN_TEST(test_put_get);
    RUN_TEST(test_end);

    g_es->end();
    
    UNITY_END();
}

void loop() {
  // 何もしない（テストは setup() 内で完結）
}
