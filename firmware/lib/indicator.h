#ifndef indicator_h
#define indicator_h

#include "console.h"

class Indicator
{
public:
  Indicator(byte _pinIndicator)
  {
    pinIndicator = _pinIndicator;
  }

  void setup()
  {
    pinMode(pinIndicator, OUTPUT);
  }

  void setLevel(byte value)
  {
    int pwm = constrain(map(value, 0, 100, 0, 255), 0, 255);
    analogWrite(pinIndicator, pwm);
    console.log(F("PWM: "), pwm);
  }

private:
  byte pinIndicator;
};

#endif
