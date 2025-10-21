#include "DrawDisplay.h"

DrawDisplay::DrawDisplay(){}

void DrawDisplay::begin(TwoWire* wireInstance, int SDA_PIN, int SCL_PIN, int width, int height, int address){
	wire = wireInstance;
    wire->setSDA(SDA_PIN);
    wire->setSCL(SCL_PIN);
	screenWidth = width;
	screenHeight = height;
	display = new Adafruit_SSD1306(screenWidth, screenHeight, wire, OLED_RESET);

	if(!display->begin(SSD1306_SWITCHCAPVCC, address)) {
		LOG_ERROR(F("[Display] allocation failed"));
		for(;;); // Don't proceed, loop forever
	}

	display->clearDisplay();
	display->display();

	LOG_INFO(F("[Display] initialized"));

}

void DrawDisplay::clearDisplay(){
	LOG_INFO(F("[Display] cleared"));
	display->clearDisplay();
	display->display();
}

void DrawDisplay::drawCentreString(const String &buf)
{
	LOG_DEBUG("[Display] drawCentreString: " + buf);
	display->clearDisplay();
	int16_t x1, y1;
	uint16_t w, h;
	display->getTextBounds(buf, screenWidth/2, screenHeight/2, &x1, &y1, &w, &h);
	display->setTextColor(SSD1306_WHITE);
	display->setCursor(screenWidth/2 - w / 2, screenHeight/2 - h / 2);
	display->print(buf);

	display->display();
}

void DrawDisplay::drawCentreNumber(const int Number){
	display->clearDisplay();

	if(Number >= 100 || Number <= -10){
		display->setTextSize(3);
	}else{
		display->setTextSize(4);
	}
	drawCentreString(String(Number));

	display->display();
}

void DrawDisplay::drawMenu(const String items[3], int cursorIndex, bool invertCursor, const String rightTexts[3]){
	
	LOG_INFO(F("[Display] drawMenu"));

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