#include "PedalsController.hpp"

PedalsController* PedalsController::self = nullptr;

void PedalsController::begin(String DeviceName) {
     // 1) 設定ロード（EEPROM -> RAM）
    settings_->begin();
    settings_->loadFromStorage();
    delay(1000);

    // 2) USB MIDI 初期化
    usbMIDI_.begin(DeviceName);
    delay(1000);

    // 3) フットスイッチ初期化
    //   - beginが本数を要するなら第二引数で渡す（APIに合わせて調整）
    //     例: pedals_->begin(pedalPins_, SettingsManager::MAX_PEDALS);
    pedals_->begin(pedalPins_);
    pedals_->attachCallback(&PedalsController::pedalsCallbackStatic);
    delay(1000);

    // 4) エクスプレッションペダル初期化
    expPedal_.begin(&MCP_I2C_INSTANCE, SDA1_PIN, SCL1_PIN);
    expPedal_.attachCallback(&PedalsController::expPedalsCallbackStatic);
    delay(1000);

    // CCPedalState_ 初期化（TOGGLE用状態）
    for (size_t i = 0; i < MAX_PEDALS-1; i++) {
        CCPedalState_[i] = false;
    }
}
void PedalsController::start() {
    // 各コンポーネントの開始処理をここに記述
}
void PedalsController::stop() {
    // 各コンポーネントの停止処理をここに記述
}
void PedalsController::update() {
    // 各コンポーネントの定期処理をここに記述
    pedals_->update();
    expPedal_.update();
    // MIDI送信などの処理もここで行う
}

void PedalsController::pedalsCallback(int index, bool state) {
    if (index < 0 || index >= MAX_PEDALS-1) return;

    // CCモードの場合の処理
    // モーメンタリ動作の場合、そのまま送信
    usbMIDI_.sendControlChange(
        1,
        index+1,
        state ? 127 : 0
    );
}
void PedalsController::expPedalCallback(int value) {
    // エクスプレッションペダルの値に基づいてMIDI CCを送信
    if (value < 0) value = 0;
    if (value > 127) value = 127;

    usbMIDI_.sendControlChange(
        1,        
        7,         
        value      
    );
}

void PedalsController::pedalsCallbackStatic(int index, bool state){
    if (self) self->pedalsCallback(index, state);
}

void PedalsController::expPedalsCallbackStatic(int value){
    if (self) self->expPedalCallback(value);
}