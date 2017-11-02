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
	NUM_BUTTONS = 2,
	NUM_EVENTS = 10
};

typedef struct event
{
	bool inUse;
	uint8_t ch;
	uint8_t val;
	uint8_t day;
	uint8_t hour;
	uint8_t min;
	uint8_t sec;
} event_t;

class Hiki
{
public:
	byte buttonPin[NUM_BUTTONS] = {3, 16};
	byte prevButtonState[NUM_BUTTONS] = {1, 1};
	byte knobPin[NUM_KNOBS] = {14, 15};
	byte chan[NUM_CHANNELS] = {9, 10, 5, 6};
	bool buttonFlag[NUM_BUTTONS] = {false, false};
	ts_t time;
	char buf[50];
	uint32_t startTime;
	event_t events[NUM_EVENTS];


	Hiki();
	void begin();
	void write(uint8_t, uint8_t);
	void on(uint8_t);
	void off(uint8_t);
	uint8_t read(uint8_t);
	bool poll();
	bool button(uint8_t button);
	void print(char);
	void print(const char *);
	void print(uint8_t );
	void print(float );
	void home();
	void setCursor(uint8_t, uint8_t);
	void clear();
	char *getTemp();
	char *getHum();
	float temperature();
	float humidity();
	bool wait(uint32_t startTime);

	void setTime(uint8_t, uint8_t, uint8_t);
	void setDate(uint16_t, uint8_t, uint8_t);
	char *getTime();
	char *getDateShort();
	char *getDateFull();
	char *getTimeAndDate(); 
	void setAlarm1(uint8_t day, uint8_t hour, uint8_t min, uint8_t sec, uint8_t alarmType);
	char *getAlarm1();
	void enableAlarm1();
	void disableAlarm1();
	void clearAlarm1();
    bool isAlarm1On();
	void setAlarm2(uint8_t day, uint8_t hour, uint8_t min, uint8_t alarmType);
	char *getAlarm2();
	void enableAlarm2();
	void disableAlarm2();
	void clearAlarm2();
    bool isAlarm2On();
    uint8_t getCtrl();
	uint8_t getStatus();

	uint8_t setEvent(uint8_t hour, uint8_t min, uint8_t chan, uint8_t val);
	static void rtcIntp();

private:
};

extern Hiki hiki;
