#include "EepromStrage.h"

// コンストラクタ
EepromStrage::EepromStrage(){
}

void EepromStrage::begin(size_t size){
    EEPROM.begin(size);
    LOG_INFO("[EepromStrage] initialized with size %d bytes", size);
}
void EepromStrage::end(){
    EEPROM.end();
    LOG_INFO("[EepromStrage] ended");
}
bool EepromStrage::commit(){
    if(EEPROM.commit()){
        LOG_INFO("[EepromStrage] committed successfully");
        return true;
    } else {
        LOG_ERROR("[EepromStrage] commit failed");
        return false;
    }
}
void EepromStrage::get(int const address, void *data, size_t size){
    uint8_t* bytes = reinterpret_cast<uint8_t*>(data);
    for (size_t i = 0; i < size; ++i) {
        bytes[i] = EEPROM.read(address + i);
    }

}
void EepromStrage::put(int const address, const void *data, size_t size){
    const uint8_t* bytes = reinterpret_cast<const uint8_t*>(data);
    for (size_t i = 0; i < size; ++i) {
        EEPROM.write(address + i, bytes[i]);
    }
}