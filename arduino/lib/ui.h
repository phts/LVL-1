#ifndef ui_h
#define ui_h

#include <TimerMs.h>
#include "indicator.h"
#include "console.h"

class UI
{
public:
  static const byte ERROR_CODE_NONE = 0;
  static const byte ERROR_CODE_SENSOR = 1;
  static const byte ERROR_CODE_TRANSPORT = 2;
  static const byte ERROR_CODE_WIFI = 3;
  static const byte ERROR_CODE_HTTP = 4;
  static const byte ERRORS_SIZE = 6;

  UI(Indicator *indicator, byte warningLevel) : _autohideErrorTimer(UI_ERROR_AUTOHIDE_DELAY, 0, 1)
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
    if (_autohideErrorTimer.tick())
    {
      hideTempError();
    }
  }

  void showLevel(byte value)
  {
    _level = value;
    console.debug(F("UI"), F("show level:"), value);
    showCachedLevel();
  }

  void showError(byte code, bool autohide = false)
  {
    console.debug(F("UI"), F("show error:"), String(F("code=")) + code + String(F(" autohide=")) + autohide);
    showErrorCode(code);
    if (autohide)
    {
      _autohideErrorTimer.restart();
    }
    else
    {
      _cachedError = code;
    }
  }

  void hideError()
  {
    console.debug(F("UI"), F("hide error"));
    _cachedError = ERROR_CODE_NONE;
    showCachedLevel();
  }

  void showBusy()
  {
    _indicator->setLed(Indicator::LED_BUSY);
  }

  void showProgressBar()
  {
    console.debug(F("UI"), F("show progress bar"));
    showBusy();
    _progress = 0;
    _indicator->setLevel(0);
  }

  void hideProgressBar()
  {
    console.debug(F("UI"), F("hide progress bar"));
    showLevel(_level);
  }

  void tickProgress()
  {
    if (_progress < 100)
    {
      _progress++;
    }
    _indicator->setLevel(_progress);
  }

  byte getProgress()
  {
    return _progress;
  }

private:
  Indicator *_indicator;
  byte _warningLevel;
  byte _level = 0;
  byte _cachedError = ERROR_CODE_NONE;
  TimerMs _autohideErrorTimer;
  byte _progress = 0;

  void showCachedLevel()
  {
    bool warning = _level >= _warningLevel;
    _indicator->setLevel(_level);
    _indicator->setLed(warning ? Indicator::LED_WARNING : Indicator::LED_OFF);
  }

  void showErrorCode(byte code)
  {

    _indicator->setLevel(100 / ERRORS_SIZE * code);
    _indicator->setLed(Indicator::LED_ERROR);
  }

  void showCachedError()
  {
    showErrorCode(_cachedError);
  }

  void hideTempError()
  {
    _cachedError == ERROR_CODE_NONE ? hideError() : showCachedError();
  }
};

#endif
