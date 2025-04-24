#ifndef ui_h
#define ui_h

#include "indicator.h"
#include "console.h"

class UI
{
public:
  static const byte ERROR_CODE_SENSOR = 1;
  static const byte ERROR_CODE_TRANSPORT = 2;
  static const byte ERROR_CODE_WIFI = 3;
  static const byte ERROR_CODE_HTTP = 4;
  static const byte ERRORS_SIZE = 6;

  UI(Indicator *indicator, int warningLevel) : errorTimer(UI_DISPLAY_ERROR_DELAY, 0, 1)
  {
    _indicator = indicator;
    _warningLevel = warningLevel;
  }

  void setup()
  {
    _indicator->setup();
  }

  void tick()
  {
    _indicator->tick();
    if (errorTimer.tick())
    {
      hideError();
    }
  }

  void showLevel(int value)
  {
    _level = value;
    console.debug(F("UI:: show level:"), value);
    bool warning = value >= _warningLevel;
    _indicator->setLevel(value);
    _indicator->setLed(warning ? Indicator::LED_WARNING : Indicator::LED_OFF);
  }

  void showError(byte code)
  {
    console.debug(F("UI:: show error:"), code);
    _indicator->setLevel(100 / ERRORS_SIZE * code);
    _indicator->setLed(Indicator::LED_ERROR);
  }

  void showTempError(byte code)
  {
    showError(code);
    errorTimer.restart();
  }

  void hideError()
  {
    console.debug(F("UI:: hide error"));
    showLevel(_level);
  }

  byte showProgressBar()
  {
    console.debug(F("UI:: show progress bar"));
    progress = 0;
    _indicator->setLevel(0);
    _indicator->setLed(Indicator::LED_INFO);
    return progress;
  }

  void hideProgressBar()
  {
    console.debug(F("UI:: hide progress bar"));
    showLevel(_level);
  }

  byte tickProgressBar()
  {
    if (progress < 100)
    {
      progress++;
    }
    _indicator->setLevel(progress);
    return progress;
  }

private:
  Indicator *_indicator;
  int _warningLevel;
  int _level = 0;
  TimerMs errorTimer;
  byte progress = 0;
};

#endif
