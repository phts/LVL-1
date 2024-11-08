#ifndef main_h
#define main_h

#include <EncButton.h>
#include "settings.h"
#include "ultrasonic.h"

Button btnCheck(PIN_BTN_CHECK);
Ultrasonic ultrasonic(PIN_ULTRASONIC_SENSOR_TRIGGER, PIN_ULTRASONIC_SENSOR_ECHO);

void btnCheckCallback()
{
  switch (btnCheck.action())
  {
  case EB_PRESS:
    float distance = ultrasonic.getDistance();
    if (distance < 0)
    {
      Serial.println('Failed to read ultrasonic sensor');
    }
    else
    {
      Serial.println(distance);
    }
    break;
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
}

#endif
