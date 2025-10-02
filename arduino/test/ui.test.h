#ifndef ui_test_h
#define ui_test_h

#include <EncButton.h>
#include "../lib/config.h"
#include "../lib/settings.h"
#include "../lib/led.h"
#include "../lib/indicator.h"
#include "../lib/ui.h"

Button btnMeasure(Config::PIN_BTN_MEASURE);
Led led(Config::PIN_LED);
Indicator indicator(&led, Config::PIN_METER);
UI ui(&indicator, LEVEL_WARNING);

byte direction = 1;
byte error = 0;

void measure(byte delta)
{
  ui.showLevel(indicator.getLevel() + delta * direction);
  Serial.println(indicator.getLevel());
  if (indicator.getLevel() == 100 || indicator.getLevel() == 0)
  {
    direction = -1 * direction;
  }
}

void btnMeasureCallback()
{
  switch (btnMeasure.action())
  {
  case EB_CLICK:
    ui.hideError();
    measure(5);
    break;
  case EB_HOLD:
    error = (error + 1) % UI::ERRORS_SIZE;
    ui.showError(error, true);
    break;
  }
}

void setup()
{
  Serial.begin(CONSOLE_SERIAL_PORT_BAUD);
  btnMeasure.attach(btnMeasureCallback);
}

void loop()
{
  btnMeasure.tick();
  ui.tick();
}

#endif
