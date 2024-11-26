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

  Indicator(Led *_led, byte _pinIndicator) : ledBlinkTimer(INDICATOR_LED_ERROR_BLINK_INTERVAL)
  {
    led = _led;
    pinIndicator = _pinIndicator;
  }

  void setup()
  {
    pinMode(pinIndicator, OUTPUT);
    led->setup();
  }

  void tick()
  {
    if (ledBlinkTimer.tick())
    {
      led->toggle();
    }
  }

  void setLevel(int value)
  {
    int pwm = constrain(map(value, 0, 100, 0, 255), 0, 255);
    analogWrite(pinIndicator, pwm);
    console.log(F("PWM: "), pwm);
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
};

#endif
