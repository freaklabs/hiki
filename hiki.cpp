#include "hiki.h"

#define LCD_ADDRESS 0x27
#define DHTPIN  4
#define DHTTYPE DHT11

Hiki hiki;

LiquidCrystal_I2C lcd(LCD_ADDRESS, 16, 2);
DHT dht(DHTPIN, DHTTYPE);

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
	for (int i=0; i<NUM_CHANNELS; i++)
	{
		pinMode(chan[i], OUTPUT);
		digitalWrite(chan[i], LOW);
	}
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