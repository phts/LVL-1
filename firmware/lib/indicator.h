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

  Indicator(Led *_led, byte _pinIndicator)
  {
    led = _led;
    pinIndicator = _pinIndicator;
  }

  void setup()
  {
    pinMode(pinIndicator, OUTPUT);
    led->setup();
    ledImpulseTimer.setTimerMode();
  }

  void tick()
  {
    if (ledBlinkTimer.tick())
    {
      ledImpulseTimer.start();
      led->write(HIGH);
    }
    if (ledImpulseTimer.tick())
    {
      led->write(LOW);
    }
  }

  void setLevel(int _level)
  {
    level = _level;
    int pwm = constrain(map(level, 0, 100, 0, 255), 0, 255);
    analogWrite(pinIndicator, pwm);
    console.log(F("PWM: "), pwm);
  }

  int getLevel()
  {
    return level;
  }

  void setLed(byte type)
  {
    ledBlinkTimer.stop();
    switch (type)
    {
    case LED_OFF:
      led->write(LOW);
      break;
    case LED_WARNING:
      led->write(HIGH);
      break;
    case LED_ERROR:
      ledImpulseTimer.setTime(INDICATOR_LED_ERROR_IMPULSE_INTERVAL);
      ledBlinkTimer.setTime(INDICATOR_LED_ERROR_BLINK_INTERVAL);
      ledBlinkTimer.start();
      break;
    case LED_INFO:
      ledImpulseTimer.setTime(INDICATOR_LED_INFO_IMPULSE_INTERVAL);
      ledBlinkTimer.setTime(INDICATOR_LED_INFO_BLINK_INTERVAL);
      ledBlinkTimer.start();
      break;
    default:
      break;
    }
  }

private:
  Led *led;
  byte pinIndicator;
  TimerMs ledBlinkTimer;
  TimerMs ledImpulseTimer;
  int level;
};

#endif
