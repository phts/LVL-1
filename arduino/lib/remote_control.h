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
  static const byte ACTION_SET_MEASURE_INTERVAL = 3;

  RemoteControl(Internet *internet) : _timer(REMOTE_CONTROL_INIT_DELAY)
  {
    _internet = internet;
  }

  void setup(OnResponseCallback cb)
  {
    _onResponse = cb;
    _timer.start();
    _currentId.reserve(14);
    _nextId.reserve(14);
    _nextActionPayload.reserve(2);
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
    _nextActionPayload = "";
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
    else if (nextAction == F("interval"))
    {
      _nextAction = ACTION_SET_MEASURE_INTERVAL;
    }
    else
    {
      _internet->sendLog(F("warn"), F("Unsupported remote action: "), nextAction);
      _nextAction = ACTION_NOTHING;
    }
  }

  void setNextActionPayload(String nextActionPayload)
  {
    console.debug(F("RemoteControl"), F("setNextActionPayload:"), nextActionPayload);
    _nextActionPayload = nextActionPayload;
  }

  void saveNext()
  {
    if (!_initialized)
    {
      console.debug(F("RemoteControl"), F("initalized: id="), _nextId);
      String msg;
      msg.reserve(52);
      msg.concat(F("Remote control module initialized: "));
      msg.concat(F("id="));
      msg.concat(_nextId);
      _internet->sendLog(F("info"), msg);
      _currentAction = ACTION_NOTHING;
      _currentId = _nextId;
      _timer.setTime(REMOTE_CONTROL_INTERVAL);
      _initialized = true;
      return;
    }
    if (_nextId != _currentId)
    {
      if (_nextAction == ACTION_NOTHING)
      {
        _currentId = _nextId;
        _currentAction = _nextAction;
        return;
      }
      console.debug(F("RemoteControl"), F("new ID saved:"), _nextId);
      String msg;
      msg.reserve(70);
      msg.concat(F("Processing remote action \""));
      msg.concat(_nextAction);
      if (_nextActionPayload.length())
      {
        msg.concat(F("\" with payload \""));
        msg.concat(_nextActionPayload);
      }
      msg.concat(F("\": id="));
      msg.concat(_nextId);
      _internet->sendLog(F("info"), msg);
      _currentAction = _nextAction;
      _currentId = _nextId;
      _currentActionPayload = _nextActionPayload;
    }
  }

  byte getAction()
  {
    return _currentAction;
  }

  String getActionPayload()
  {
    return _currentActionPayload;
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
  String _nextActionPayload;
  String _currentActionPayload;
  bool _initialized = false;
};

#endif
