#include <Arduino.h>
#include <unity.h>
#include <DrawDisplay.hpp>
#include <config.h>
#include <Wire.h>

#define DISPLAY_TIME 1000  // 各描画結果の表示時間（ms）

// テスト対象
static DrawDisplay* g_dd = nullptr;

// ------- ユーティリティ --------

// I2Cの疎通確認（デバイスが見つからなければスキップ用に true を返す）
static bool oled_missing() {
	Wire.setSDA(SDA0_PIN);
	Wire.setSCL(SCL0_PIN);
	Wire.begin();
	delay(5);

	Wire.beginTransmission(DEFAULT_SCREEN_ADDRESS);
	uint8_t err = Wire.endTransmission();
	return (err != 0);  // 0 以外なら見つからない
}

// ------- テスト本体 --------

void test_draw_center_string_various() {
	// 例: 短い/長い/日本語/記号など
	g_dd->drawCentreString("A");
	delay(DISPLAY_TIME);
	g_dd->drawCentreString("Hello, OLED!");
	delay(DISPLAY_TIME);
	g_dd->drawCentreString("テスト");
	delay(DISPLAY_TIME);
	g_dd->drawCentreString("Symbols: +-*/=#@");
	delay(DISPLAY_TIME);

	// クラッシュせず描画できれば合格（バッファ検査はライブラリ非公開のため省略）
	TEST_PASS_MESSAGE("drawCentreString completed without error");
}

void test_draw_center_number_boundaries() {
  // フォントサイズ切替の境界値を一通り叩く（内部で setTextSize(3/4)）
	g_dd->drawCentreNumber(-10);
	delay(DISPLAY_TIME);
	g_dd->drawCentreNumber(-9);
	delay(DISPLAY_TIME);
	g_dd->drawCentreNumber(0);
	delay(DISPLAY_TIME);
	g_dd->drawCentreNumber(9);
	delay(DISPLAY_TIME);
	g_dd->drawCentreNumber(10);
	delay(DISPLAY_TIME);
	g_dd->drawCentreNumber(99);
	delay(DISPLAY_TIME);
	g_dd->drawCentreNumber(100);
	delay(DISPLAY_TIME);

	TEST_PASS_MESSAGE("drawCentreNumber completed without error");
}

void test_draw_menu_variations() {
	const String items[3]      = {"A", "B", "C"};
	const String rightOn[3]    = {"  ON", "  -", "  +5"};
	const String rightEmpty[3] = {"", "", ""};

	// invert なし
	for (int idx = 0; idx < 3; ++idx) {
		g_dd->drawMenu(items, idx, /*invert*/false, rightEmpty);
		delay(DISPLAY_TIME);
	}
	// invert あり（カーソル行の前景/背景反転）
	for (int idx = 0; idx < 3; ++idx) {
		g_dd->drawMenu(items, idx, /*invert*/true, rightOn);
		delay(DISPLAY_TIME);
	}
	TEST_PASS_MESSAGE("drawMenu completed without error");
}

// 必要なら setUp/tearDown で毎テストの前後処理を入れられます
void setUp() {
}
void tearDown() {
    g_dd->clearDisplay();
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

	// OLED が見つからなければスキップ
	if (oled_missing()) {
		TEST_IGNORE_MESSAGE("OLED device not found on I2C. Skipping on-device display tests.");
		UNITY_END();
		return;
	}

	// テスト対象の生成＆初期化
	DrawDisplay dd(&OLED_I2C_INSTANCE, SDA0_PIN, SCL0_PIN, SCREEN_WIDTH, SCREEN_HEIGHT);
	g_dd = &dd;

	// 注意: init() 内で begin() 失敗時は for(;;) で止まる実装なので、
	// ここに来る時点で I2C 疎通が取れていることを事前に確認しています。
	g_dd->begin();

	// ---- テスト実行 ----
	RUN_TEST(test_draw_center_string_various);
	RUN_TEST(test_draw_center_number_boundaries);
	RUN_TEST(test_draw_menu_variations);

	UNITY_END();
}

void loop() {
  // 何もしない（テストは setup() 内で完結）
}
