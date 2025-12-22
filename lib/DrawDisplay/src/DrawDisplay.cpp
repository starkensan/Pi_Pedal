#include "DrawDisplay.hpp"

DrawDisplay::DrawDisplay(TwoWire* wireInstance, int SDA_PIN, int SCL_PIN, int width, int height, int address)
: SDA_PIN(SDA_PIN), SCL_PIN(SCL_PIN), ADDRESS(address), screenWidth(width), screenHeight(height) {
	wire = wireInstance;
}

void DrawDisplay::begin(){
	wire->setSDA(SDA_PIN);
	wire->setSCL(SCL_PIN);

	display = new Adafruit_SSD1306(screenWidth, screenHeight, wire, OLED_RESET);

	if(!display->begin(SSD1306_SWITCHCAPVCC, ADDRESS)) {
		for(;;); // Don't proceed, loop forever
	}

	display->clearDisplay();
	display->display();

}

void DrawDisplay::clearDisplay(){
	display->clearDisplay();
	display->display();
}

void DrawDisplay::drawCentreString(const String &buf)
{
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

void DrawDisplay::drawMenu(const String items[DRAW_MENU_MAX_ITEMS], int cursorIndex, bool invertCursor, const String rightTexts[3]){

	display->clearDisplay();
	display->setTextSize(2);
	display->setTextColor(SSD1306_WHITE);

	for(int i=0; i<DRAW_MENU_MAX_ITEMS; i++){
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