#ifndef startup_h
#define startup_h

#include "settings.h"
#include "console.h"
#include "indicator.h"
#include "internet.h"

class Startup
{
public:
  Startup(Indicator *indicator, Internet *internet) : _startupTimer(STARTUP_TIMER_INTERVAL, 0, 0)
  {
    _indicator = indicator;
    _internet = internet;
  }

  void setup(OnResponseCallback connectCallback)
  {
    console.info(F("PHTS LVL-1"));
    console.info(F("Staring up..."));
    _starting = true;
    _connectCallback = connectCallback;
    _indicator->setLed(Indicator::LED_INFO);
    _startupTimer.start();
  }

  void tick()
  {
    if (_startupTimer.tick())
    {
      if (_indicator->getLevel() < _maxProgress)
      {
        _indicator->setLevel(_indicator->getLevel() + 1);
      }
      if (_indicator->getLevel() == 10)
      {
        _internet->connect(_connectCallback);
      }
      if (_indicator->getLevel() == 100)
      {
        _startupTimer.stop();
        _starting = false;
      }
    }
  }

  bool isStarting()
  {
    return _starting;
  }

  void setMaxProgress(int value)
  {
    _maxProgress = value;
  }

private:
  Indicator *_indicator;
  Internet *_internet;
  TimerMs _startupTimer;
  OnResponseCallback _connectCallback;
  bool _starting;
  int _maxProgress = 90;
};

#endif
