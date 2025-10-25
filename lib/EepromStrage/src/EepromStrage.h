#ifndef EepromStrage_h
#define EepromStrage_h

#include<Arduino.h>
#include <DebugLog.h>
#include <EEPROM.h>

class EepromStrage{
	public:
		EepromStrage();

        void begin(size_t size);
        void end();
        bool commit();
        void get(int const address, void *data, size_t size);
        void put(int const address, const void *data, size_t size);
        void clear();

	private:

};

#endif