#include "Pedal.hpp"

// コンストラクタ
Pedal::Pedal(unsigned long debounceDelay) 
    :debounceDelay(debounceDelay) {
}

void Pedal::begin(const int (&pins)[MAX_PEDALS-1], bool isPullup){
  	// スイッチのピンを設定し、初期状態を読み込む
	std::copy(pins, pins + (MAX_PEDALS-1), pins_);
    for(int i=0; i<MAX_PEDALS-1; i++){
		pinMode(pins_[i], isPullup ? INPUT_PULLUP:INPUT_PULLDOWN);
		currentState[i] = digitalRead(pins_[i]);
		previousState[i] = currentState[i];
		lastChangeTime[i] = 0; // 初期化
	}
	LOG_INFO("[Pedal] Pedal on pin initialized. isPullup : %d", isPullup);
}

void Pedal::attachCallback(void (*callback)(int, bool)){
    pedalCallback = callback;
    LOG_DEBUG("[Pedal] Attach callback to pedal");
}

// 状態チェックとコールバックの呼び出し
void Pedal::update() {
	for(int i=0; i<MAX_PEDALS-1; i++){
		int reading = digitalRead(pins_[i]);
		unsigned long currentTime = millis();
		
		// 状態が変わっており、かつデバウンス時間が経過している場合のみ処理
		if (reading != previousState[i] && (currentTime - lastChangeTime[i] >= debounceDelay)) {
			currentState[i] = reading;
			lastChangeTime[i] = currentTime; // 状態変更時間を記録
			pedalCallback(i, currentState[i]);  // コールバック呼び出し
			previousState[i] = currentState[i];
		}
	}
}

int Pedal::getPin(int PedalNum){
    return pins_[PedalNum];
}

bool Pedal::getState(int PedalNum){
    return currentState[PedalNum] == HIGH;
}