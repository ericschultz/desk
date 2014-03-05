

#ifndef LCDADAPTER_H
#define LCDADAPTER_H
#include "LiquidCrystal.h"

#include "Arduino.h"
#include "stdio.h"


class LcdAdapter
{
	public:

		LcdAdapter(uint8_t rs, uint8_t rw, uint8_t enable,
			uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3, int marqueeLength);
		LcdAdapter(uint8_t rs, uint8_t enable,
			uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3, int marqueeLength);
		void begin(uint8_t cols, uint8_t rows);
		void handleEvents();
		size_t print(String text);
		size_t marquee(String text);
		~LcdAdapter();
	private:
		LiquidCrystal * innerLcd;
        int marqueeTime;
		String printText;
		String marqueeText;
		unsigned long beganMarqueeTime;
		bool inMarquee;

};


#endif

