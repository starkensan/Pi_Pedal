#include "Pedal.hpp"

// コンストラクタ
Pedal::Pedal(unsigned long debounceDelay) 
    :debounceDelay(debounceDelay) {
}

void Pedal::begin(const int (&pins)[MAX_PEDALS-1], bool isPullup){
  	// スイッチのピンを設定し、初期状態を読み込む
	isPullup_ = isPullup;

    for(int i=0; i<MAX_PEDALS-1; i++){
		pins_[i] = pins[i];
		pinMode(pins_[i], isPullup ? INPUT_PULLUP:INPUT_PULLDOWN);
		currentState[i] = digitalRead(pins_[i]) ^ isPullup_;
		lastChangeTime[i] = 0; // 初期化
	}
	LOG_INFO("[Pedal] Pedal initialized. isPullup : ", isPullup);
}

void Pedal::attachCallback(void (*callback)(int, bool)){
    pedalCallback = callback;
	LOG_INFO("[Pedal] Attached callback");
}

// 状態チェックとコールバックの呼び出し
void Pedal::update() {
	for(int i=0; i<MAX_PEDALS-1; i++){
		int reading = digitalRead(pins_[i]) ^ isPullup_;
		unsigned long currentTime = millis();
		
		// 状態が変わっており、かつデバウンス時間が経過している場合のみ処理
		if (reading != currentState[i] && (currentTime - lastChangeTime[i] >= debounceDelay)) {
			currentState[i] = reading;
			lastChangeTime[i] = currentTime; // 状態変更時間を記録
			LOG_INFO("[Pedal] Pedal ", i, "changed state: ", reading);
			if(pedalCallback != nullptr)pedalCallback(i, currentState[i]);  // コールバック呼び出し
		}
	}
}

int Pedal::getPin(int index){
    return pins_[index];
}

bool Pedal::getState(int index){
    return currentState[index] == HIGH;
}