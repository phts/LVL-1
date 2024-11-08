#include <HCSR04.h>
#include "settings.h"

UltraSonicDistanceSensor distanceSensor(PIN_ULTRASONIC_SENSOR_TRIGGER, PIN_ULTRASONIC_SENSOR_ECHO);

void setup()
{
  Serial.begin(9600);
}

void loop()
{
  float distance = distanceSensor.measureDistanceCm();
  if (distance < 0)
  {
    Serial.println('Failed to read ultrasonic sensor');
  }
  else
  {
    Serial.println(distance);
  }
  delay(1000);
}
