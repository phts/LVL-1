#ifndef main_h
#define main_h

#include <EncButton.h>
#include "config.h"
#include "settings.h"
#include "ultrasonic.h"
#include "level.h"
#include "helpers.h"
#include "console.h"

Button btnCheck(PIN_BTN_CHECK);
Ultrasonic ultrasonic(PIN_ULTRASONIC_SENSOR_TRIGGER, PIN_ULTRASONIC_SENSOR_ECHO);
Level level(PIN_INDICATOR, PIN_LED_WARNING, LEVEL_WARNING);

void btnCheckCallback()
{
  switch (btnCheck.action())
  {
  case EB_PRESS:
    float distance = ultrasonic.getDistance();
    if (distance < 0)
    {
      console.log(F("Failed to read ultrasonic sensor"));
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
}

#endif
