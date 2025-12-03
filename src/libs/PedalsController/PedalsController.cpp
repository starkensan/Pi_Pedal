#include "PedalsController.hpp"

PedalsController* PedalsController::self = nullptr;

void PedalsController::begin(String DeviceName) {
     // 設定ロード（EEPROM -> RAM）
    settings_.begin();

    // USB MIDI 初期化
    usbMIDI_.begin(DeviceName);
    delay(1000);

    // フットスイッチ初期化
    pedals_.begin(pedalPins_);
    pedals_.attachCallback(&PedalsController::pedalsCallbackStatic);

    // エクスプレッションペダル初期化
    expPedal_.begin(&MCP_I2C_INSTANCE, SDA1_PIN, SCL1_PIN);
    expPedal_.attachCallback(&PedalsController::expPedalsCallbackStatic);

    // CCPedalState_ 初期化（TOGGLE用状態）
    for (size_t i = 0; i < MAX_PEDALS-1; i++) {
        CCPedalState_[i] = false;
    }
}

void PedalsController::start() {
    midiStarted_ = true;
}

void PedalsController::stop() {
    midiStarted_ = false;
}

void PedalsController::update() {
    pedals_.update();
    expPedal_.update();
}

void PedalsController::pedalsCallback(int index, bool state) {

    if (midiStarted_ == false) {
        return;
    }else{
        if (index < 0 || index >= MAX_PEDALS-1) return;

        if (settings_.getPedalSettings(index).pedalMode == SettingsManager::PedalMode::CC) {
            // Control Change
            if (settings_.getPedalSettings(index).switchBehavior == SettingsManager::SwitchBehavior::TOGGLE) {
                if (state) {
                    CCPedalState_[index] = !CCPedalState_[index];
                    usbMIDI_.sendControlChange(
                        settings_.getPedalSettings(index).midiChannel,
                        settings_.getPedalSettings(index).ccNumber,
                        CCPedalState_[index] ? 127 : 0
                    );
                }
                return;
            }else if(settings_.getPedalSettings(index).switchBehavior == SettingsManager::SwitchBehavior::MOMENTARY){
                // モーメンタリ動作の場合、そのまま送信
                usbMIDI_.sendControlChange(
                    settings_.getPedalSettings(index).midiChannel,
                    settings_.getPedalSettings(index).ccNumber,
                    state ? 127 : 0
                );
            }
        }else if(settings_.getPedalSettings(index).pedalMode == SettingsManager::PedalMode::PC_NEXT){
            // Program Change Next
            if(state){
                PCCurrentNumber_++;
                if (PCCurrentNumber_ > 127) PCCurrentNumber_ = 0; // 0-127の範囲に制限
                usbMIDI_.sendProgramChange(
                    settings_.getPedalSettings(index).midiChannel,
                    PCCurrentNumber_
                );
            }
        }else if(settings_.getPedalSettings(index).pedalMode == SettingsManager::PedalMode::PC_BACK){
            // Program Change Back
            if(state){
                if (PCCurrentNumber_ == 0) {
                    PCCurrentNumber_ = 127;
                } else {
                    PCCurrentNumber_--;
                }
                usbMIDI_.sendProgramChange(
                    settings_.getPedalSettings(index).midiChannel,
                    PCCurrentNumber_
                );
            }
        }
    }

}
void PedalsController::expPedalCallback(int value) {

    if (midiStarted_ == false) {
        return;
    }else{
        // エクスプレッションペダルの値に基づいてMIDI CCを送信
        if (value < 0) value = 0;
        if (value > 127) value = 127;

        usbMIDI_.sendControlChange(
            settings_.getPedalSettings(MAX_PEDALS-1).midiChannel,
            settings_.getPedalSettings(MAX_PEDALS-1).ccNumber,         
            value      
        );
    }

}

void PedalsController::pedalsCallbackStatic(int index, bool state){
    if (self) self->pedalsCallback(index, state);
}

void PedalsController::expPedalsCallbackStatic(int value){
    if (self) self->expPedalCallback(value);
}