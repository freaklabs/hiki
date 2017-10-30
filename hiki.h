#pragma once

#include "LiquidCrystal_I2C.h"
#include "Arduino.h"
#include "DHT.h"
#include "rtc3231.h"
#include <limits.h>

enum
{
	NUM_CHANNELS = 4,
	NUM_KNOBS = 2,
	NUM_BUTTONS = 2
};

class Hiki
{
public:
	byte buttonPin[2] = {3, 16};
	byte prevButtonState[2] = {1, 1};
	byte knobPin[2] = {14, 15};
	byte chan[4] = {9, 10, 5, 6};
	bool buttonFlag[2] = {false, false};
	ts_t time;
	char buf[50];
	uint32_t startTime;


	Hiki();
	void begin();
	void write(uint8_t, uint8_t);
	void on(uint8_t);
	void off(uint8_t);
	uint8_t read(uint8_t);
	void poll();
	bool button(uint8_t button);
	void print(char);
	void print(const char *);
	void print(uint8_t );
	void print(float );
	void home();
	void setCursor(uint8_t, uint8_t);
	void clear();
	void setTime(uint8_t, uint8_t, uint8_t);
	void setDate(uint16_t, uint8_t, uint8_t);
	char *getTime();
	char *getDateShort();
	char *getDateFull();
	char *getTimeAndDate(); 
	char *getTemp();
	char *getHum();
	float temperature();
	float humidity();
	bool wait(uint32_t startTime);
//	void setAlarm();
//	void getAlarm();

private:
};

extern Hiki hiki;
