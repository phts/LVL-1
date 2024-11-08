#ifndef main_h
#define main_h

#include <EncButton.h>
#include "settings.h"
#include "ultrasonic.h"
#include "level.h"

Button btnCheck(PIN_BTN_CHECK);
Ultrasonic ultrasonic(PIN_ULTRASONIC_SENSOR_TRIGGER, PIN_ULTRASONIC_SENSOR_ECHO);
Level level;

void btnCheckCallback()
{
  switch (btnCheck.action())
  {
  case EB_PRESS:
    float distance = ultrasonic.getDistance();
    level.setValue(distance);
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
