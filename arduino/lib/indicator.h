#ifndef indicator_h
#define indicator_h

#include <TimerMs.h>
#include "settings.h"
#include "console.h"
#include "led.h"

class Indicator
{
public:
  static const byte LED_OFF = 0;
  static const byte LED_WARNING = 1;
  static const byte LED_ERROR = 2;
  static const byte LED_INFO = 3;

  Indicator(Led *led, byte pinMeter)
  {
    _led = led;
    _pinMeter = pinMeter;
  }

  void setup()
  {
    pinMode(_pinMeter, OUTPUT);
    _led->setup();
    _ledImpulseTimer.setTimerMode();
  }

  void tick()
  {
    if (_ledBlinkTimer.tick())
    {
      _ledImpulseTimer.start();
      _led->write(HIGH);
    }
    if (_ledImpulseTimer.tick())
    {
      _led->write(LOW);
    }
  }

  void setLevel(int level)
  {
    _level = level;
    int pwm = constrain(map(_level, 0, 100, 0, INDICATOR_METER_MAX_PWM), 0, INDICATOR_METER_MAX_PWM);
    analogWrite(_pinMeter, pwm);
    console.debug(F("Indicator:: PWM:"), pwm);
  }

  int getLevel()
  {
    return _level;
  }

  void setLed(byte type)
  {
    console.debug(F("Indicator:: LED:"), type);
    _ledBlinkTimer.stop();
    switch (type)
    {
    case LED_OFF:
      _led->write(LOW);
      break;
    case LED_WARNING:
      _led->write(HIGH);
      break;
    case LED_ERROR:
      _ledImpulseTimer.setTime(INDICATOR_LED_ERROR_IMPULSE_INTERVAL);
      _ledBlinkTimer.setTime(INDICATOR_LED_ERROR_BLINK_INTERVAL);
      _ledBlinkTimer.start();
      break;
    case LED_INFO:
      _ledImpulseTimer.setTime(INDICATOR_LED_INFO_IMPULSE_INTERVAL);
      _ledBlinkTimer.setTime(INDICATOR_LED_INFO_BLINK_INTERVAL);
      _ledBlinkTimer.start();
      break;
    default:
      break;
    }
  }

private:
  Led *_led;
  byte _pinMeter;
  TimerMs _ledBlinkTimer;
  TimerMs _ledImpulseTimer;
  int _level;
};

#endif
