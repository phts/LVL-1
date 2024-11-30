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
  if (resp.startsWith(F("progress!=")))
  {
    startup.setMaxProgress(resp.substring(10).toInt());
  }
  if (resp == TRANSPORT_SUCCESS_RESPONSE)
  {
    startup.setMaxProgress(100);
  }
}

void reportError(String log)
{
  indicator.setLed(Indicator::LED_ERROR);
  console.info(log);
}

void internetErrorCallback(String command, byte type, String desc)
{
  reportError(command + String(F(": ")) + desc);
  if (command.startsWith(F("!level=")))
  {
    internet.disconnect([](String _)
                        { internet.connect(nullptr); });
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
    reportError(F("Failed to read ultrasonic sensor"));
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
  internet.setup(internetErrorCallback);
  startup.setup(connectCallback);
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
