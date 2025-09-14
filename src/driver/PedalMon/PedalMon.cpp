#include "PedalMon.h"

// コンストラクタ
PedalMon::PedalMon(midi::MidiInterface<midi::SerialMIDI<Adafruit_USBD_MIDI>>* midiInstance, const int* pins, const int* PedalControlNumbers, int SwitchesNumber, unsigned long debounceDelay) 
    : midi(midiInstance), pins(pins), controlNumbers(PedalControlNumbers), numSwitches(SwitchesNumber), debounceDelay(debounceDelay) {
    
    // スイッチの状態を保持する配列を初期化
    currentStates = new int[numSwitches];
    previousStates = new int[numSwitches];
    lastChangeTimes = new unsigned long[numSwitches]; // デバウンス用の時間配列
}

PedalMon::~PedalMon() {
    delete[] currentStates;
    delete[] previousStates;
    delete[] lastChangeTimes;
}

void PedalMon::begin(){
  // スイッチのピンを設定し、初期状態を読み込む
    for (int i = 0; i < numSwitches; i++) {
        pinMode(pins[i], INPUT_PULLUP);
        currentStates[i] = digitalRead(pins[i]);
        previousStates[i] = currentStates[i];
        lastChangeTimes[i] = 0; // 初期化
    }
}

// 状態チェックとコールバックの呼び出し
void PedalMon::update() {
    for (int i = 0; i < numSwitches; i++) {
        int reading = digitalRead(pins[i]);
        unsigned long currentTime = millis();
        
        // 状態が変わっており、かつデバウンス時間が経過している場合のみ処理
        if (reading != previousStates[i] && (currentTime - lastChangeTimes[i] >= debounceDelay)) {
            currentStates[i] = reading;
            lastChangeTimes[i] = currentTime; // 状態変更時間を記録
            sendSwitchesMIDI(i, currentStates[i]);  // コールバック呼び出し
            previousStates[i] = currentStates[i];
        }
    }
}

void PedalMon::sendSwitchesMIDI(int switchIndex, int state){
  midi->sendControlChange(controlNumbers[switchIndex], state ? 0 : 127, 1);
}
