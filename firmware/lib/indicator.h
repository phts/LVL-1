#ifndef indicator_h
#define indicator_h

#include "console.h"

#define INDICATOR_WARN_OFF 0
#define INDICATOR_WARN_ON 1

class Indicator
{
public:
  Indicator(byte _pinIndicator, byte _pinLedWarning)
  {
    pinIndicator = _pinIndicator;
    pinLedWarning = _pinLedWarning;
  }

  void setup()
  {
    pinMode(pinIndicator, OUTPUT);
    pinMode(pinLedWarning, OUTPUT);
  }

  void setLevel(byte value)
  {
    int pwm = constrain(map(value, 0, 100, 0, 255), 0, 255);
    analogWrite(pinIndicator, pwm);
    console.log(F("PWM: "), pwm);
  }

  void setWarning(byte type)
  {
    switch (type)
    {
    case INDICATOR_WARN_OFF:
      digitalWrite(pinLedWarning, LOW);
      break;
    case INDICATOR_WARN_ON:
      digitalWrite(pinLedWarning, HIGH);
      break;
    default:
      break;
    }
  }

private:
  byte pinIndicator;
  byte pinLedWarning;
};

#endif
