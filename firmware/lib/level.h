#ifndef level_h
#define level_h

#include "indicator.h"
#include "console.h"

class Level
{
public:
  Level(byte _pinIndicator, byte _pinLedWarning, int _warningLevel) : indicator(_pinIndicator)
  {
    pinLedWarning = _pinLedWarning;
    warningLevel = _warningLevel;
  }

  void setup()
  {
    indicator.setup();
    pinMode(pinLedWarning, OUTPUT);
  }

  void setValue(int value)
  {
    console.log(F("Level: "), value);
    bool warning = value >= warningLevel;
    digitalWrite(pinLedWarning, warning ? HIGH : LOW);
    indicator.setLevel(value);
  }

private:
  Indicator indicator;
  byte pinLedWarning;
  int warningLevel;
};

#endif
