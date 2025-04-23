#ifndef indicator_test_h
#define indicator_test_h

#include <EncButton.h>
#include "../lib/config.h"
#include "../lib/indicator.h"

Button btnCheck(Config::PIN_BTN_CHECK);
Led led(Config::PIN_LED);
Indicator indicator(&led, Config::PIN_METER);
int direction = 1;

void check()
{
  indicator.setLevel(indicator.getLevel() + 1 * direction);
  Serial.println(indicator.getLevel());
  if (indicator.getLevel() == 100 || indicator.getLevel() == 0)
  {
    direction = -1 * direction;
    indicator.setLed(indicator.getLevel() == 100 ? Indicator::LED_WARNING : Indicator::LED_OFF);
  }
}

void btnCheckCallback()
{
  switch (btnCheck.action())
  {
  case EB_PRESS:
    check();
  }
}

void setup()
{
  Serial.begin(SERIAL_PORT);
  btnCheck.attach(btnCheckCallback);
}

void loop()
{
  btnCheck.tick();
  indicator.tick();
}

#endif
