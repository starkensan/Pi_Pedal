#include "ExpPedal.h"

ExpPedal::ExpPedal(int thresholdValue)
  : threshold(thresholdValue){
}

void ExpPedal::begin(TwoWire* wireInstance, int sdaPin, int sclPin, uint8_t address) {
	if (sdaPin >= 0 && sclPin >= 0) {
		wireInstance->setSDA(sdaPin);
		wireInstance->setSCL(sclPin);
	}
	wireInstance->begin();
	mcp.begin(address, wireInstance);
	mcp.setGain(GAIN_1X);
	mcp.setResolution(RESOLUTION_14_BIT);
	mcp.setMode(MODE_CONTINUOUS);
}

void ExpPedal::attachCallback(void (*callback)(int value)) {
  	expPedalCallback = callback;
}

void ExpPedal::update() {
	// Update Expression Pedal
	if (mcp.isReady()) {
		uint16_t rawValue = constrain(mcp.readADC(), 0, 6600);
		int value = map(rawValue, 0, 6600, 0, 128);

		if (abs(value - previousValue) > threshold) {
		expPedalCallback(value);
		previousValue = value;
		}
	}
}

int ExpPedal::getValue() {
	return previousValue;
}