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
#include "wifi.h"
#include "startup.h"

Button btnCheck(Config::PIN_BTN_CHECK);
Ultrasonic ultrasonic(Config::PIN_ULTRASONIC_SENSOR_TRIGGER, Config::PIN_ULTRASONIC_SENSOR_ECHO);
Led led(Config::PIN_LED);
Indicator indicator(&led, Config::PIN_INDICATOR);
Level level(&indicator, LEVEL_WARNING);
Wifi wifi(Config::PIN_WIFI_RX, Config::PIN_WIFI_TX);
Startup startup(&indicator, &wifi);

TimerMs checkTimer(CHECK_INITIAL_DELAY);

void connectCallback(String resp)
{
  if (resp.startsWith(F("progress=")))
  {
    startup.setMaxProgress(resp.substring(9).toInt());
  }
  if (resp == TRANSPORT_SUCCESS_RESPONSE)
  {
    startup.setMaxProgress(100);
  }
}

void check()
{
  if (startup.isStarting())
  {
    console.log(F("Not started yet. Check skipped..."));
    return;
  }
  checkTimer.setTime(CHECK_INTERVAL);
  checkTimer.start();
  float distance = ultrasonic.getDistance();
  if (distance < 0)
  {
    console.log(F("Failed to read ultrasonic sensor"));
    indicator.setLed(Indicator::LED_ERROR);
    return;
  }
  console.log(F("Distance: "), distance);
  int lvl = helpers.distanceToLevel(distance);
  level.setValue(lvl);
  wifi.sendLevel(lvl);
}

void btnCheckCallback()
{
  switch (btnCheck.action())
  {
  case EB_PRESS:
    check();
  }
}

void setup()
{
  Serial.begin(SERIAL_PORT);
  btnCheck.attach(btnCheckCallback);
  level.setup();
  checkTimer.start();
  wifi.setup(connectCallback);
  startup.setup();
}

void loop()
{
  btnCheck.tick();
  indicator.tick();
  if (checkTimer.tick())
  {
    check();
  }
  wifi.tick();
  startup.tick();
}

#endif
