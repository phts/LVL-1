#ifndef level_h
#define level_h

#include "config.h"
#include "settings.h"

class Level
{
public:
  Level()
  {
  }

  void setup()
  {
    pinMode(PIN_LED_WARNING, OUTPUT);
  }

  void setValue(float _value)
  {
    float value = _value;
    bool warning = value < LEVEL_WARNING;
    digitalWrite(PIN_LED_WARNING, warning ? HIGH : LOW);
    if (value < 0)
    {
      Serial.println('Failed to read ultrasonic sensor');
    }
    else
    {
      Serial.println(value);
    }
  }
};

#endif
