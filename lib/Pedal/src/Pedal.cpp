#include "Pedal.h"

// コンストラクタ
Pedal::Pedal(const int pin ,unsigned long debounceDelay) 
    :pin(pin), debounceDelay(debounceDelay) {
}

void Pedal::begin(bool isPullup){
  // スイッチのピンを設定し、初期状態を読み込む
    pinMode(pin, isPullup ? INPUT_PULLUP:INPUT_PULLDOWN);
    currentState = digitalRead(pin);
    previousState = currentState;
    lastChangeTime = 0; // 初期化
    LOG_INFO("Pedal on pin %d initialized. isPullup : %d", pin, isPullup);
}

void Pedal::attachCallback(void (*callback)(bool)){
    pedalCallback = callback;
    LOG_DEBUG("attach callback to pedal on pin %d", pin);
}

// 状態チェックとコールバックの呼び出し
void Pedal::update() {
    int reading = digitalRead(pin);
    unsigned long currentTime = millis();
    
    // 状態が変わっており、かつデバウンス時間が経過している場合のみ処理
    if (reading != previousState && (currentTime - lastChangeTime >= debounceDelay)) {
        currentState = reading;
        lastChangeTime = currentTime; // 状態変更時間を記録
        pedalCallback(currentState);  // コールバック呼び出し
        previousState = currentState;
    }
}

int Pedal::getPin(){
    return pin;
}

bool Pedal::getState(){
    return currentState;
}