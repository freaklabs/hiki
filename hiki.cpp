#include "hiki.h"

#define LCD_ADDRESS 0x3F
#define DHTPIN  4
#define DHTTYPE DHT11

#define DEBUG 0

// allows printing or not printing based on the DEBUG VAR
#if (DEBUG == 1)
  #define DBG_PRINT(...)   Serial.print(__VA_ARGS__)
  #define DBG_PRINTLN(...) Serial.println(__VA_ARGS__)
#else
  #define DBG_PRINT(...)
  #define DBG_PRINTLN(...)
#endif

Hiki hiki;

LiquidCrystal_I2C lcd(LCD_ADDRESS, 16, 2);
DHT dht(DHTPIN, DHTTYPE);
static volatile bool rtcFlag = false;

/*************************************************************

*************************************************************/
Hiki::Hiki()
{

}

/*************************************************************

*************************************************************/
void Hiki::begin()
{
    startTime = millis();
    lcd.begin();
    rtc.begin();
    dht.begin();
    attachInterrupt(0, Hiki::rtcIntp, FALLING);
    memset(&events, 0, sizeof(events));

    clearAlarm1();
    setAlarm1(0, 0, 0, 0, EVERY_SECOND);
    enableAlarm1();

	for (int i=0; i<NUM_CHANNELS; i++)
	{
		pinMode(chan[i], OUTPUT);
		digitalWrite(chan[i], LOW);
	}
}

/*************************************************************

*************************************************************/
void Hiki::rtcIntp()
{
    rtcFlag = true;
}

/*************************************************************

*************************************************************/
bool Hiki::poll()
{
    uint8_t val[2];
    uint8_t ret;

    for (int i=0; i<NUM_BUTTONS; i++)
    {
        val[i] = digitalRead(buttonPin[i]);
        if (val[i] != prevButtonState[i])
        {
            if (val[i] == LOW)
            {
                buttonFlag[i] = true;
                delay(10);
            }
        }   
        prevButtonState[i] = val[i]; 
    }

    // check to see if the rtcFlag went off. If it did, then check to see
    // if we have an event match
    if (rtcFlag == true)
    {
        clearAlarm1();
        rtc.getTime(&time);
        DBG_PRINTLN(getTimeAndDate());
        ret = rtcFlag;
        rtcFlag = false;

        for (int i=0; i<NUM_EVENTS; i++)
        {
            if (events[i].inUse)
            {
                if ((events[i].hour == time.hour) && (events[i].min == time.min) && (time.sec == 0))
                {
                    analogWrite(chan[events[i].ch], events[i].val);
                    DBG_PRINTLN("Event processed");
                }
            }
        }
    }
    return ret;
}

/*************************************************************

*************************************************************/
void Hiki::write(uint8_t channel, uint8_t val)
{
    if (channel < NUM_CHANNELS)
    {
        analogWrite(chan[channel], val);
    }
}

/*************************************************************

*************************************************************/
void Hiki::on(uint8_t channel)
{
    if (channel < NUM_CHANNELS)
    {
        analogWrite(chan[channel], 255);
    }
}

/*************************************************************

*************************************************************/
void Hiki::off(uint8_t channel)
{
    if (channel < NUM_CHANNELS)
    {
        analogWrite(chan[channel], 0);
    }
}

/*************************************************************

*************************************************************/
uint8_t Hiki::read(uint8_t knob)
{
    uint16_t val = analogRead(knob);
    val = ~val;
    return (val >> 2);
}

/*************************************************************

*************************************************************/
bool Hiki::button(uint8_t button)
{
    bool val = buttonFlag[button];
    buttonFlag[button] = false;
    delay(3);
    return val;
}

/*************************************************************

*************************************************************/
void Hiki::print(const char *msg)
{
    lcd.print(msg);
}

/*************************************************************

*************************************************************/
void Hiki::print(uint8_t val)
{
    lcd.print(val);
}

/*************************************************************

*************************************************************/
void Hiki::print(float val)
{
    lcd.print(val);
}

/*************************************************************

*************************************************************/
void Hiki::print(char val)
{
    lcd.print(val);
}

/*************************************************************

*************************************************************/
void Hiki::setCursor(uint8_t col, uint8_t row)
{
    lcd.setCursor(col, row);
}

/*************************************************************

*************************************************************/
void Hiki::home()
{
    lcd.home();
}


/*************************************************************

*************************************************************/
void Hiki::clear()
{
    lcd.clear();
}

/*************************************************************

*************************************************************/
void Hiki::setTime(uint8_t hour, uint8_t min, uint8_t sec)
{
    rtc.getTime(&time);
    time.hour = hour;
    time.min = min;
    time.sec = sec;
    rtc.setTime(&time);
}

/*************************************************************

*************************************************************/
void Hiki::setDate(uint16_t year, uint8_t mon, uint8_t day)
{
    rtc.getTime(&time);
    time.year = year;
    time.mon = mon;
    time.mday = day;
    rtc.setTime(&time);
}

/*************************************************************

*************************************************************/
char *Hiki::getTime()
{
    rtc.getTime(&time);
    sprintf(buf, "%02d:%02d:%02d", time.hour, time.min, time.sec);
    return buf;
}/*************************************************************

*************************************************************/
char *Hiki::getDateShort()
{
    rtc.getTime(&time);
    sprintf(buf, "%02d/%02d", time.mon, time.mday);
    return buf;
}

/*************************************************************

*************************************************************/
char *Hiki::getDateFull()
{
    rtc.getTime(&time);
    sprintf(buf, "%04d/%02d/%02d", time.year, time.mon, time.mday);
    return buf;
}

/*************************************************************

*************************************************************/
char *Hiki::getTimeAndDate()
{
    rtc.getTime(&time);
    sprintf(buf, "%02d/%02d/%02d %02d:%02d:%02d", time.year, time.mon, time.mday, time.hour, time.min, time.sec);
    return buf;
}

/*************************************************************

*************************************************************/
float Hiki::temperature()
{
    return dht.readTemperature();
}

/*************************************************************

*************************************************************/
char *Hiki::getTemp()
{
    uint8_t val = dht.readTemperature();
    sprintf(buf, "%02d", val);
    return buf; 
}

/*************************************************************

*************************************************************/
float Hiki::humidity()
{
    return dht.readHumidity();
}

/*************************************************************

*************************************************************/
char *Hiki::getHum()
{
    uint8_t val = dht.readHumidity();
    sprintf(buf, "%02d", val);
    return buf; 
}

/************************************************************************/
// elapsedTime - calculates time elapsed from startTime
// startTime : time to start calculating
/************************************************************************/
bool Hiki::wait(uint32_t delay)
{
  uint32_t stopTime = millis();
  
  if (stopTime >= startTime)
  {
    if ((stopTime - startTime) > delay)
    {
        startTime = millis();
        return true;
    }
    else
    {
        return false;
    }
  }
  else
  {
    if ((ULONG_MAX - (startTime - stopTime)) > delay)
    {
        startTime = millis();
        return true;
    }
    else
    {
        return false;
    }
  }
}

/*************************************************************

*************************************************************/
void Hiki::setAlarm1(uint8_t day, uint8_t hour, uint8_t min, uint8_t sec, uint8_t alarmType)
{
    rtc.setAlarm1(day, hour, min, sec, alarmType);
}

/*************************************************************

*************************************************************/
char *Hiki::getAlarm1()
{
    rtc.getAlarm1(buf, 50);
    return buf;
}

/*************************************************************

*************************************************************/
void Hiki::enableAlarm1()
{
    rtc.enableAlarm1();
}

/*************************************************************

*************************************************************/
void Hiki::disableAlarm1()
{
    rtc.disableAlarm1();
}

/*************************************************************

*************************************************************/
void Hiki::clearAlarm1()
{
    rtc.clearAlarm1();
}

/*************************************************************

*************************************************************/
bool Hiki::isAlarm1On()
{
    return rtc.isAlarm1On();
}

/*************************************************************

*************************************************************/
void Hiki::setAlarm2(uint8_t day, uint8_t hour, uint8_t min, uint8_t alarmType)
{
    rtc.setAlarm2(day, hour, min, alarmType);
}

/*************************************************************

*************************************************************/
char *Hiki::getAlarm2()
{
    rtc.getAlarm2(buf, 50);
    return buf;
}

/*************************************************************

*************************************************************/
void Hiki::enableAlarm2()
{
    rtc.enableAlarm2();
}

/*************************************************************

*************************************************************/
void Hiki::disableAlarm2()
{
    rtc.disableAlarm2();
}

/*************************************************************

*************************************************************/
void Hiki::clearAlarm2()
{
    rtc.clearAlarm2();
}

/*************************************************************

*************************************************************/
bool Hiki::isAlarm2On()
{
    return rtc.isAlarm2On();
}


/*************************************************************

*************************************************************/
uint8_t Hiki::getCtrl()
{
    return rtc.getCtrl();
}

/*************************************************************

*************************************************************/
uint8_t Hiki::getStatus()
{
    return rtc.getStatus();
}

/*************************************************************

*************************************************************/
uint8_t Hiki::setEvent(uint8_t hour, uint8_t min, uint8_t chan, uint8_t val)
{
    for (int i=0; i<NUM_EVENTS; i++)
    {
        if (!events[i].inUse)
        {
            events[i].inUse = true;
            events[i].ch = chan;
            events[i].val = val;
            events[i].hour = hour;
            events[i].min = min;
            return i;
        }
    }
    return 255;
}
