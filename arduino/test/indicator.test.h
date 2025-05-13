#ifndef indicator_test_h
#define indicator_test_h

#include <EncButton.h>
#include "../lib/config.h"
#include "../lib/indicator.h"

Button btnMeasure(Config::PIN_BTN_MEASURE);
Led led(Config::PIN_LED);
Indicator indicator(&led, Config::PIN_METER);
int direction = 1;

void measure()
{
  indicator.setLevel(indicator.getLevel() + 1 * direction);
  Serial.println(indicator.getLevel());
  if (indicator.getLevel() == 100 || indicator.getLevel() == 0)
  {
    direction = -1 * direction;
    indicator.setLed(indicator.getLevel() == 100 ? Indicator::LED_WARNING : Indicator::LED_OFF);
  }
}

void btnMeasureCallback()
{
  switch (btnMeasure.action())
  {
  case EB_PRESS:
    measure();
  }
}

void setup()
{
  Serial.begin(SERIAL_PORT);
  btnMeasure.attach(btnMeasureCallback);
}

void loop()
{
  btnMeasure.tick();
  indicator.tick();
}

#endif
