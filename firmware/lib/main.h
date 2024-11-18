#ifndef main_h
#define main_h

#include <EncButton.h>
#include "config.h"
#include "settings.h"
#include "ultrasonic.h"
#include "level.h"
#include "helpers.h"
#include "console.h"
#include "indicator.h"
#include "led.h"

Button btnCheck(PIN_BTN_CHECK);
Ultrasonic ultrasonic(PIN_ULTRASONIC_SENSOR_TRIGGER, PIN_ULTRASONIC_SENSOR_ECHO);
Led warningLed(PIN_LED_WARNING);
Indicator indicator(&warningLed, PIN_INDICATOR);
Level level(&indicator, LEVEL_WARNING);

void btnCheckCallback()
{
  switch (btnCheck.action())
  {
  case EB_PRESS:
    float distance = ultrasonic.getDistance();
    if (distance < 0)
    {
      console.log(F("Failed to read ultrasonic sensor"));
      indicator.setWarning(INDICATOR_WARN_GENERIC_ERROR);
      break;
    }
    else
    {
      console.log(F("Distance: "), distance);
    }
    level.setValue(helpers.distanceToLevel(distance));
    break;
  }
}

void setup()
{
  Serial.begin(SERIAL_PORT);
  btnCheck.attach(btnCheckCallback);
  level.setup();
}

void loop()
{
  btnCheck.tick();
  indicator.tick();
}

#endif
