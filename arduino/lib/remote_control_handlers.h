#ifndef remote_control_handlers_h
#define remote_control_handlers_h

#include <TimerMs.h>
#include "remote_control.h"
#include "internet.h"
#include "ui.h"

typedef void (*Handler)();

class RemoteControlHandlers
{
public:
  RemoteControlHandlers(RemoteControl *remoteControl)
  {
    this->remoteControl = remoteControl;
  }

  void setup(Handler actionMeasure,
             Handler actionMeasureAndResetTimer,
             Handler actionSetMeasureInterval,
             Handler actionLedOff)
  {
    this->actionMeasure = actionMeasure;
    this->actionMeasureAndResetTimer = actionMeasureAndResetTimer;
    this->actionSetMeasureInterval = actionSetMeasureInterval;
    this->actionLedOff = actionLedOff;
  }

  void tick()
  {
    byte action = this->remoteControl->getAction();
    if (action == RemoteControl::ACTION_NOTHING)
    {
      return;
    }
    this->remoteControl->markAsProcessed();
    switch (action)
    {
    case RemoteControl::ACTION_MEASURE:
      this->actionMeasure();
      break;
    case RemoteControl::ACTION_MEASURE_AND_RESET_TIMER:
      this->actionMeasureAndResetTimer();
      break;
    case RemoteControl::ACTION_SET_MEASURE_INTERVAL:
      this->actionSetMeasureInterval();
      break;
    case RemoteControl::ACTION_LED_OFF:
      this->actionLedOff();
      break;
    }
  }

private:
  RemoteControl *remoteControl;
  Handler actionMeasure;
  Handler actionMeasureAndResetTimer;
  Handler actionSetMeasureInterval;
  Handler actionLedOff;
};

#endif
