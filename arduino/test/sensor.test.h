#ifndef indicator_test_h
#define indicator_test_h

#include <EncButton.h>
#include <HCSR04.h>
#include <TimerMs.h>
#include "../lib/settings.h"
#include "../lib/config.h"
#include "../lib/console.h"

bool power;
Button btnMeasure(Config::PIN_BTN_MEASURE);
TimerMs samplesTimer(ULTRASONIC_SAMPLES_INTERVAL);
UltraSonicDistanceSensor sensor(Config::PIN_ULTRASONIC_SENSOR_TRIGGER, Config::PIN_ULTRASONIC_SENSOR_ECHO);

void on()
{
  console.debug(F("Ultrasonic"), F("power=on"));
  digitalWrite(Config::PIN_ULTRASONIC_SENSOR_POWER, HIGH);
  power = true;
}

void off()
{
  console.debug(F("Ultrasonic"), F("power=off"));
  digitalWrite(Config::PIN_ULTRASONIC_SENSOR_POWER, LOW);
  power = false;
}

void toggle()
{
  if (samplesTimer.active())
  {
    samplesTimer.stop();
  }
  else
  {
    samplesTimer.restart();
  }
}

void btnMeasureCallback()
{
  switch (btnMeasure.action())
  {
  case EB_CLICK:
    toggle();
    break;
  case EB_HOLD:
    if (power)
    {
      off();
    }
    else
    {
      on();
    }
    break;
  }
}

void setup()
{
  Serial.begin(CONSOLE_SERIAL_PORT_BAUD);
  btnMeasure.attach(btnMeasureCallback);
  pinMode(Config::PIN_ULTRASONIC_SENSOR_POWER, OUTPUT);
  on();
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
