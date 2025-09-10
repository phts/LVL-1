#ifndef indicator_test_h
#define indicator_test_h

#include <EncButton.h>
#include <HCSR04.h>
#include <TimerMs.h>
#include "../lib/settings.h"
#include "../lib/config.h"
#include "../lib/console.h"

Button btnMeasure(Config::PIN_BTN_MEASURE);
TimerMs samplesTimer(ULTRASONIC_SAMPLES_INTERVAL);
UltraSonicDistanceSensor sensor(Config::PIN_ULTRASONIC_SENSOR_TRIGGER, Config::PIN_ULTRASONIC_SENSOR_ECHO);

void measure()
{
  samplesTimer.restart();
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
  if (samplesTimer.tick())
  {
    float distance = sensor.measureDistanceCm();
    console.debug(F("Ultrasonic"), F("sample="), distance);
  }
}

#endif
