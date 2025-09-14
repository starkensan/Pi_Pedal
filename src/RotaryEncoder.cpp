#include <sys/types.h>
#include "RotaryEncoder.h"

RotaryEncoder::RotaryEncoder(int DT, int CLK, int SW, int RotaryDebounceTime, int SwitchDebounceTime) {
  _ROTARYDEBOUNCE = RotaryDebounceTime;
  _SWITCHDEBOUNCE = SwitchDebounceTime;
  _DTPIN = DT;
  _CLKPIN = CLK;
  _SWPIN = SW;

  pinMode(_DTPIN, INPUT);
  pinMode(_CLKPIN, INPUT);
  pinMode(_SWPIN, INPUT);
}

void RotaryEncoder::checkEncoder() {

  uint8_t currentDtState = digitalRead(_DTPIN);
  uint8_t currentClkState = digitalRead(_CLKPIN);

  static uint8_t lastDtState = currentDtState;
  static uint8_t lastClkState = currentClkState;

  uint8_t changeFlag = false;
  

  if (lastDtState != currentDtState || lastClkState != currentClkState) {
    if (micros() - _rotaryChangeTime >= (unsigned long)(_ROTARYDEBOUNCE)) {
      _rotaryChangeTime = micros();
      if (!currentDtState && !currentClkState) {
        changeFlag = true;
        if (lastDtState) {
          _direction = 1;
          _count++;
        } else if (lastClkState) {
          _direction = -1;
          _count--;
        }
      }
    }

    lastDtState = currentDtState;
    lastClkState = currentClkState;
  }

  if (changeFlag == true) {
    changeFlag = false;
    _rotaryCallback();
  }
}

void RotaryEncoder::checkSwitch(){
  uint8_t currentSwState = digitalRead(_SWPIN);

  static uint8_t lastSwState = currentSwState;
  static unsigned long switchChangeTime = millis();

  uint8_t changeFlag = false;
  
  if(currentSwState != lastSwState){
    if(millis() - switchChangeTime > (unsigned long)(_SWITCHDEBOUNCE)){
      switchChangeTime = millis();
      lastSwState = currentSwState;
      if(currentSwState == true){
        changeFlag = true;
      }
    }
  }

  if(changeFlag == true){
    changeFlag = false;
    _swCallback();
  }
  
}

void RotaryEncoder::attachRotaryCallback(void (*rotaryCallback)()) {
  _rotaryCallback = rotaryCallback;
}

void RotaryEncoder::attachSwitchCallback(void (*swCallback)()){
  _swCallback = swCallback;
}

int RotaryEncoder::getDirection() {
  return _direction;
}

int RotaryEncoder::getCount() {
  return _count;
}

void RotaryEncoder::setCount(int count) {
  _count = count;
}

void RotaryEncoder::clearCount() {
  _count = 0;
}