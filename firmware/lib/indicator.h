#ifndef indicator_h
#define indicator_h

#include <TimerMs.h>
#include "console.h"
#include "led.h"

#define INDICATOR_WARN_OFF 0
#define INDICATOR_WARN_ON 1
#define INDICATOR_WARN_GENERIC_ERROR 2

#define SETTINGS_INDICATOR_ERROR_BLINK_INTERVAL 500

class Indicator
{
public:
  Indicator(Led *_warningLed, byte _pinIndicator) : blinkTimer(SETTINGS_INDICATOR_ERROR_BLINK_INTERVAL)
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

  void setLevel(byte value)
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
    case INDICATOR_WARN_OFF:
      warningLed->write(LOW);
      break;
    case INDICATOR_WARN_ON:
      warningLed->write(HIGH);
      break;
    case INDICATOR_WARN_GENERIC_ERROR:
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
