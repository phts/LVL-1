#ifndef startup_h
#define startup_h

#include "settings.h"
#include "console.h"
#include "ui.h"
#include "internet.h"

class Startup
{
public:
  Startup(UI *ui, Internet *internet) : _startupTimer(STARTUP_TIMER_INTERVAL)
  {
    _ui = ui;
    _internet = internet;
  }

  void setup(OnResponseCallback connectCallback)
  {
    console.info(F("PHTS LVL-1"));
    console.info(F("Staring up..."));
    _starting = true;
    _connectCallback = connectCallback;
    _progress = _ui->showProgressBar();
    _startupTimer.start();
  }

  void tick()
  {
    if (_startupTimer.tick())
    {
      if (_progress < _maxProgress)
      {
        _progress = _ui->tickProgressBar();
      }
      if (_progress == 10)
      {
        _internet->connect(_connectCallback);
      }
      if (_progress == 100)
      {
        _startupTimer.stop();
        _internet->sendLog(F("info"), F("Started"));
        _starting = false;
      }
    }
  }

  bool isStarting()
  {
    return _starting;
  }

  void setMaxProgress(byte value)
  {
    _maxProgress = value;
  }

private:
  UI *_ui;
  Internet *_internet;
  TimerMs _startupTimer;
  OnResponseCallback _connectCallback;
  bool _starting;
  byte _progress;
  byte _maxProgress = 90;
};

#endif
