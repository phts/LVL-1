#ifndef level_h
#define level_h

#include "indicator.h"
#include "console.h"

class Level
{
public:
  Level(Indicator *indicator, int warningLevel)
  {
    _indicator = indicator;
    _warningLevel = warningLevel;
  }

  void setup()
  {
    _indicator->setup();
  }

  void setValue(int value)
  {
    console.debug(F("Level:: Value:"), value);
    bool warning = value >= _warningLevel;
    _indicator->setLevel(value);
    _indicator->setLed(warning ? Indicator::LED_WARNING : Indicator::LED_OFF);
  }

private:
  Indicator *_indicator;
  int _warningLevel;
};

#endif
