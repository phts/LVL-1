#ifndef sensor_test_h
#define sensor_test_h

#include <EncButton.h>
#include <TimerMs.h>
#include "../lib/sensor_lib_shim.h"
#include "../lib/settings.h"
#include "../lib/config.h"

#define PROD_ENV false
#if PROD_ENV
#define TEST_BTN_MEASURE Config::PIN_BTN_MEASURE
#define TEST_SENSOR_TRIGGER Config::PIN_ULTRASONIC_SENSOR_TRIGGER
#define TEST_SENSOR_ECHO Config::PIN_ULTRASONIC_SENSOR_ECHO
#define TEST_SENSOR_POWER Config::PIN_ULTRASONIC_SENSOR_POWER
#else
#define TEST_BTN_MEASURE 5
#define TEST_SENSOR_TRIGGER 11
#define TEST_SENSOR_ECHO 12
#define TEST_SENSOR_POWER 10
#endif

bool power;
Button btnMeasure(TEST_BTN_MEASURE);
TimerMs samplesTimer(ULTRASONIC_SAMPLES_INTERVAL);
Sensor sensor(TEST_SENSOR_TRIGGER, TEST_SENSOR_ECHO);

void on()
{
  Serial.println(String(F("Ultrasonic")) + F("power=on"));
  digitalWrite(TEST_SENSOR_POWER, HIGH);
  power = true;
}

void off()
{
  Serial.println(String(F("Ultrasonic")) + F("power=off"));
  digitalWrite(TEST_SENSOR_POWER, LOW);
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
    samplesTimer.start();
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
  pinMode(TEST_SENSOR_POWER, OUTPUT);
  on();
  toggle();
}

void loop()
{
  btnMeasure.tick();
  if (samplesTimer.tick())
  {
    float distance = sensor.measureDistanceCm();
    Serial.println(String(F("Ultrasonic")) + F("sample=") + distance);
  }
}

#endif
