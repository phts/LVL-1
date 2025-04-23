#ifndef ui_h
#define ui_h

#include "indicator.h"
#include "console.h"

class UI
{
public:
  static const byte ERROR_CODE_SENSOR = 1;
  static const byte ERROR_CODE_HTTP = 2;
  static const byte ERRORS_SIZE = 6;

  UI(Indicator *indicator, int warningLevel)
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
  }

  void showLevel(int value)
  {
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

private:
  Indicator *_indicator;
  int _warningLevel;
};

#endif
