#ifndef level_h
#define level_h

#include "indicator.h"
#include "console.h"

class Level
{
public:
  Level(Indicator *_indicator, int _warningLevel)
  {
    indicator = _indicator;
    warningLevel = _warningLevel;
  }

  void setup()
  {
    indicator->setup();
  }

  void setValue(int value)
  {
    console.debug(F("Level:: Value:"), value);
    bool warning = value >= warningLevel;
    indicator->setLevel(value);
    indicator->setLed(warning ? Indicator::LED_WARNING : Indicator::LED_OFF);
  }

private:
  Indicator *indicator;
  int warningLevel;
};

#endif
