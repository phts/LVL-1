#ifndef startup_h
#define startup_h

#include "settings.h"
#include "console.h"
#include "indicator.h"
#include "wifi.h"

class Startup
{
public:
  Startup(Indicator *_indicator, Wifi *_wifi) : startupTimer(STARTUP_TIMER_INTERVAL, 0, 0)
  {
    indicator = _indicator;
    wifi = _wifi;
  }

  void setup()
  {
    console.log(F("PHTS LVL-1"));
    console.log(F("Staring up..."));
    starting = true;
    indicator->setLed(Indicator::LED_INFO);
    startupTimer.start();
    wifi->connect();
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
  Wifi *wifi;
  TimerMs startupTimer;
  bool starting;
  int maxProgress = 90;
};

#endif
