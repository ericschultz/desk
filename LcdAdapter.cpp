#include "LcdAdapter.h"

LcdAdapter::LcdAdapter(uint8_t rs, uint8_t rw, uint8_t enable,
			uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3, int marquee){
	innerLcd = new LiquidCrystal(rs, rw, enable, d0, d1, d2, d3);
	marqueeTime = marquee;
}

LcdAdapter::LcdAdapter(uint8_t rs, uint8_t enable,
			uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3, int marquee){
	innerLcd = new LiquidCrystal(rs, enable, d0, d1, d2, d3);
	marqueeTime = marquee;
}

LcdAdapter::~LcdAdapter()
{
	delete innerLcd;
	innerLcd = NULL;
}

void LcdAdapter::handleEvents(){
	if (inMarquee && ((beganMarqueeTime + marqueeTime) < millis())){
		innerLcd->clear();
		innerLcd->print(printText);
		inMarquee = false;
	}
}

size_t LcdAdapter::print(String text){
	printText = text;
	if (!inMarquee){
		innerLcd->clear();
		innerLcd->print(printText);
	}
	
}

size_t LcdAdapter::marquee(String text){
	marqueeText = text;
	inMarquee = true;
	beganMarqueeTime = millis();
	innerLcd->clear();
	innerLcd->print(marqueeText);
}

void LcdAdapter::begin(uint8_t cols, uint8_t rows){
	innerLcd->begin(cols, rows);
}
