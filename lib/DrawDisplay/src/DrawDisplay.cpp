#include "DrawDisplay.h"

DrawDisplay::DrawDisplay(TwoWire* wireInstance, int SDA_PIN, int SCL_PIN) : wire(wireInstance){
    Wire.setSDA(SDA_PIN);
    Wire.setSCL(SCL_PIN);
}

void DrawDisplay::init(int screenWidth, int screenHeight){
		display = new Adafruit_SSD1306(screenWidth, screenHeight, wire, OLED_RESET);

	if(!display->begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
		Serial.println(F("SSD1306 allocation failed"));
		for(;;); // Don't proceed, loop forever
	}

	display->display();

}

void DrawDisplay::drawCentreString(const String &buf)
{
	int16_t x1, y1;
	uint16_t w, h;
	display->getTextBounds(buf, SCREEN_WIDTH/2, SCREEN_HEIGHT/2, &x1, &y1, &w, &h);
	display->setTextColor(SSD1306_WHITE);
	display->setCursor(SCREEN_WIDTH/2 - w / 2, SCREEN_HEIGHT/2 - h / 2);
	display->print(buf);
}

void DrawDisplay::drawCentreNumber(const int Number){

	if(Number >= 100 || Number <= -10){
		display->setTextSize(3);
	}else{
		display->setTextSize(4);
	}
	drawCentreString(String(Number));
}

void DrawDisplay::drawMenu(const String items[3], int cursorIndex, bool invertCursor, const String rightTexts[3]){
	
	display->clearDisplay();
	display->setTextSize(2);
	display->setTextColor(SSD1306_WHITE);

	for(int i=0; i<3; i++){
		display->setCursor(FONT_WIDTH*2, (FONT_HEIGHT*3 + 4)*i);
		display->print(items[i]);
		if(invertCursor && cursorIndex == i)display->setTextColor(SSD1306_BLACK, SSD1306_WHITE);
		display->print(rightTexts[i]);
		if(invertCursor && cursorIndex == i)display->setTextColor(SSD1306_WHITE);
	}

	display->setCursor(0, (FONT_HEIGHT*3 + 4)*(cursorIndex));
	display->print(">");

	display->display();
}