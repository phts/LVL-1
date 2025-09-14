#ifndef remote_control_h
#define remote_control_h

#include <TimerMs.h>
#include "settings.h"
#include "console.h"
#include "internet.h"

class RemoteControl
{
public:
  static const byte ACTION_NOTHING = 0;
  static const byte ACTION_MEASURE = 1;
  static const byte ACTION_MEASURE_AND_RESET_TIMER = 2;

  RemoteControl(Internet *internet) : _timer(REMOTE_CONTROL_INTERVAL)
  {
    _internet = internet;
  }

  void setup(OnResponseCallback cb)
  {
    _onResponse = cb;
    _timer.start();
    _currentId.reserve(14);
    _nextId.reserve(14);
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

  void setNextId(String nextId)
  {
    console.debug(F("RemoteControl"), F("setNextId:"), nextId);
    _nextId = nextId;
  }

  void setNextAction(String nextAction)
  {
    console.debug(F("RemoteControl"), F("setNextAction:"), nextAction);
    if (nextAction == F("measure"))
    {
      _nextAction = ACTION_MEASURE;
    }
    else if (nextAction == F("measure+reset"))
    {
      _nextAction = ACTION_MEASURE_AND_RESET_TIMER;
    }
  }

  void saveNext()
  {
    if (!_initialized)
    {
      console.debug(F("RemoteControl"), F("initalized: id="), _nextId);
      String msg;
      msg.concat(F("Remote control module initialized: "));
      msg.concat(F("id="));
      msg.concat(_nextId);
      _internet->sendLog(F("info"), msg);
      _currentAction = ACTION_NOTHING;
      _currentId = _nextId;
      _initialized = true;
      return;
    }
    if (_nextId != _currentId)
    {
      console.debug(F("RemoteControl"), F("new ID saved:"), _nextId);
      String msg;
      msg.concat(F("Processing remote action: id="));
      msg.concat(_nextId);
      _internet->sendLog(F("info"), msg);
      _currentAction = _nextAction;
      _currentId = _nextId;
    }
  }

  byte getAction()
  {
    return _currentAction;
  }

  void markAsProcessed()
  {
    console.debug(F("RemoteControl"), F("marked as processed"));
    _currentAction = ACTION_NOTHING;
  }

private:
  TimerMs _timer;
  Internet *_internet;
  OnResponseCallback _onResponse;
  byte _nextAction = ACTION_NOTHING;
  byte _currentAction = ACTION_NOTHING;
  String _nextId;
  String _currentId;
  bool _initialized = false;
};

#endif
