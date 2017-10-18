#pragma once
#include "Arduino.h"


class Hiki
{
public:
	byte knobPin[] = {A0, A1};
	byte chan[] = {0, 1, 2, 3};
	byte dhtPin = 4;

	Hiki();
	void begin();
	void dim(uint8_t, uint8_t);
	void on(uint8_t);
	void off(uint8_t);
	int read(uint8_t);
	void lcdPrint(char *);
	void lcdSetCursor(uint8_t, uint8_t);
	void lcdClear();
	void setTime(uint8_t, uint8_t, uint8_t);
	void setdate(uint16_t, uint8_t, uint8_t);
	char *readTime();
	char *readDate();
//	void setAlarm();
//	void getAlarm();

private:
};

extern Hiki hiki;
