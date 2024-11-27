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
#include "internet.h"
#include "startup.h"

Button btnCheck(Config::PIN_BTN_CHECK);
Ultrasonic ultrasonic(Config::PIN_ULTRASONIC_SENSOR_TRIGGER, Config::PIN_ULTRASONIC_SENSOR_ECHO);
Led led(Config::PIN_LED);
Indicator indicator(&led, Config::PIN_INDICATOR);
Level level(&indicator, LEVEL_WARNING);
Internet internet(Config::PIN_MODEM_RX, Config::PIN_MODEM_TX);
Startup startup(&indicator, &internet);

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
    console.info(F("Not started yet. Check skipped..."));
    return;
  }
  checkTimer.setTime(CHECK_INTERVAL);
  checkTimer.start();
  float distance = ultrasonic.getDistance();
  if (distance < 0)
  {
    console.info(F("Failed to read ultrasonic sensor"));
    indicator.setLed(Indicator::LED_ERROR);
    return;
  }
  int lvl = helpers.distanceToLevel(distance);
  level.setValue(lvl);
  internet.sendLevel(lvl);
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
  checkTimer.attach(check);
  checkTimer.start();
  internet.setup(connectCallback);
  startup.setup();
}

void loop()
{
  btnCheck.tick();
  indicator.tick();
  checkTimer.tick();
  internet.tick();
  startup.tick();
}

#endif
