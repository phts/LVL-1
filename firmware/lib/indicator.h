#ifndef indicator_h
#define indicator_h

#include <TimerMs.h>
#include "settings.h"
#include "console.h"
#include "led.h"

class Indicator
{
public:
  static const byte WARN_OFF = 0;
  static const byte WARN_ON = 1;
  static const byte WARN_GENERIC_ERROR = 2;

  Indicator(Led *_warningLed, byte _pinIndicator) : blinkTimer(INDICATOR_ERROR_BLINK_INTERVAL)
  {
    warningLed = _warningLed;
    pinIndicator = _pinIndicator;
  }

  void setup()
  {
    pinMode(pinIndicator, OUTPUT);
    warningLed->setup();
  }

  void tick()
  {
    if (blinkTimer.tick())
    {
      warningLed->toggle();
    }
  }

  void setLevel(int value)
  {
    int pwm = constrain(map(value, 0, 100, 0, 255), 0, 255);
    analogWrite(pinIndicator, pwm);
    console.log(F("PWM: "), pwm);
  }

  void setWarning(byte type)
  {
    blinkTimer.stop();
    switch (type)
    {
    case WARN_OFF:
      warningLed->write(LOW);
      break;
    case WARN_ON:
      warningLed->write(HIGH);
      break;
    case WARN_GENERIC_ERROR:
      blinkTimer.start();
      break;
    default:
      break;
    }
  }

private:
  Led *warningLed;
  byte pinIndicator;
  TimerMs blinkTimer;
};

#endif
