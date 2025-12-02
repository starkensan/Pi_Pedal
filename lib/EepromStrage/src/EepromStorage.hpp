#ifndef EEPROM_STORAGE_HPP
#define EEPROM_STORAGE_HPP

#include <Arduino.h>
#include <EEPROM.h>
#include <HalStorage.hpp>

class EepromStorage : public HalStorage {
public:
    EepromStorage() : _size(0) {}

    void begin(size_t size) override {
        _size = size;
        EEPROM.begin(size);
    }

    void end() override {
        EEPROM.end();
    }

    bool commit() override {
        return EEPROM.commit();
    }

    void get(int address, void* data, size_t size) override {
        uint8_t* p = reinterpret_cast<uint8_t*>(data);
        for (size_t i = 0; i < size; i++) {
            p[i] = EEPROM.read(address + i);
        }
    }

    void put(int address, const void* data, size_t size) override {
        const uint8_t* p = reinterpret_cast<const uint8_t*>(data);
        for (size_t i = 0; i < size; i++) {
            EEPROM.write(address + i, p[i]);
        }
        EEPROM.commit();
    }

    void clear() override {
        for (size_t i = 0; i < _size; i++) {
            EEPROM.write(i, 0xFF);
        }
        EEPROM.commit();
    }

private:
    size_t _size;
};

#endif
