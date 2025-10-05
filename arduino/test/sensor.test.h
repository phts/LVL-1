#ifndef indicator_test_h
#define indicator_test_h

#include <HCSR04.h>
#include <TimerMs.h>
#include "../lib/settings.h"
#include "../lib/config.h"
#include "../lib/console.h"

TimerMs samplesTimer(ULTRASONIC_SAMPLES_INTERVAL);
UltraSonicDistanceSensor sensor(9, 10);

void setup()
{
  Serial.begin(CONSOLE_SERIAL_PORT_BAUD);
  samplesTimer.restart();
}

void loop()
{
  if (samplesTimer.tick())
  {
    float distance = sensor.measureDistanceCm();
    console.debug(F("Ultrasonic"), F("sample="), distance);
  }
}

#endif
