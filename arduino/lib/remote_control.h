#ifndef remote_control_h
#define remote_control_h

#include <TimerMs.h>
#include "settings.h"
#include "console.h"
#include "internet.h"

class RemoteControl
{
public:
  RemoteControl(Internet *internet) : _timer(REMOTE_CONTROL_INTERVAL)
  {
    _internet = internet;
  }

  void setup(OnResponseCallback cb)
  {
    _onResponse = cb;
    _timer.start();
  }

  void tick()
  {
    if (!_timer.tick())
    {
      return;
    }
    console.debug(F("RemoteControl"), F("check"));
    _internet->getRemoteControl(_onResponse);
  }

private:
  TimerMs _timer;
  Internet *_internet;
  OnResponseCallback _onResponse;
};

#endif
