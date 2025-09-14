#include "ExpressionPedal.h"

ExpressionPedal::ExpressionPedal(midi::MidiInterface<midi::SerialMIDI<Adafruit_USBD_MIDI>>* midiInstance, int midiCCExp, int thresholdValue)
  : midi(midiInstance), midiCCExpression(midiCCExp), threshold(thresholdValue){
  previousCCValue = -1;
}

void ExpressionPedal::begin(TwoWire* wireInstance, int sdaPin, int sclPin, uint8_t address) {
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

void ExpressionPedal::ChagneControlNumber(int ControlNumber){
  midiCCExpression = ControlNumber;
}

void ExpressionPedal::update() {
  // Update Expression Pedal
  if (mcp.isReady()) {
    uint16_t rawValue = mcp.readADC();
    int ccValue = map(rawValue, 0, 6700, 0, 128);

    if (abs(ccValue - previousCCValue) > threshold) {
      sendExpressionMIDI(ccValue);
      previousCCValue = ccValue;
    }
  }
}

void ExpressionPedal::sendExpressionMIDI(int ccValue) {
  midi->sendControlChange(midiCCExpression, ccValue, 1); // チャンネル1で送信
}
