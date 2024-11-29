#ifndef startup_h
#define startup_h

#include "settings.h"
#include "console.h"
#include "indicator.h"
#include "internet.h"

class Startup
{
public:
  Startup(Indicator *_indicator, Internet *_internet) : startupTimer(STARTUP_TIMER_INTERVAL, 0, 0)
  {
    indicator = _indicator;
    internet = _internet;
  }

  void setup(void (*connectCallback)(String))
  {
    console.info(F("PHTS LVL-1"));
    console.info(F("Staring up..."));
    starting = true;
    indicator->setLed(Indicator::LED_INFO);
    startupTimer.start();
    internet->connect(connectCallback);
  }

  void tick()
  {
    if (startupTimer.tick())
    {
      if (indicator->getLevel() < maxProgress)
      {
        indicator->setLevel(indicator->getLevel() + 1);
      }
      if (indicator->getLevel() == 100)
      {
        startupTimer.stop();
        starting = false;
      }
    }
  }

  bool isStarting()
  {
    return starting;
  }

  void setMaxProgress(int value)
  {
    maxProgress = value;
  }

private:
  Indicator *indicator;
  Internet *internet;
  TimerMs startupTimer;
  bool starting;
  int maxProgress = 90;
};

#endif
