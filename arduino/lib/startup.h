#ifndef startup_h
#define startup_h

#include <TimerMs.h>
#include "settings.h"
#include "console.h"
#include "ui.h"
#include "internet.h"

typedef void (*OnStartedCallback)();

class Startup
{
public:
  Startup(UI *ui, Internet *internet) : _startupTimer(STARTUP_TIMER_INTERVAL)
  {
    _ui = ui;
    _internet = internet;
  }

  void setup(OnResponseCallback connectCallback, OnStartedCallback startedCallback)
  {
    console.info(F("Starting up..."));
    _connectCallback = connectCallback;
    _startedCallback = startedCallback;
    _ui->showProgressBar();
    _startupTimer.start();
  }

  void tick()
  {
    if (!_startupTimer.tick())
    {
      return;
    }
    if (_ui->getProgress() < _maxProgress)
    {
      _ui->tickProgress();
    }
    if (_ui->getProgress() == 10)
    {
      _internet->connect(_connectCallback);
    }
    if (_ui->getProgress() == 100)
    {
      _startupTimer.stop();
      _startedCallback();
    }
  }

  bool isStarting()
  {
    return _startupTimer.active();
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
  OnStartedCallback _startedCallback;
  byte _maxProgress = 90;
};

#endif
