#ifndef main_h
#define main_h

#include <EncButton.h>
#include <Response.h>
#include "config.h"
#include "settings.h"
#include "ultrasonic.h"
#include "ui.h"
#include "helpers.h"
#include "console.h"
#include "indicator.h"
#include "led.h"
#include "internet.h"
#include "startup.h"

Button btnCheck(Config::PIN_BTN_CHECK);
Ultrasonic ultrasonic(Config::PIN_ULTRASONIC_SENSOR_TRIGGER, Config::PIN_ULTRASONIC_SENSOR_ECHO);
Led led(Config::PIN_LED);
Indicator indicator(&led, Config::PIN_METER);
UI ui(&indicator, LEVEL_WARNING);
Internet internet(Config::PIN_MODEM_RX, Config::PIN_MODEM_TX);
Startup startup(&indicator, &internet);

TimerMs checkTimer(CHECK_INITIAL_DELAY);

void connectCallback(String resp)
{
  if (Response::equals(resp, F("progress!")))
  {
    startup.setMaxProgress(Response::valueOf(resp).toInt());
  }
  if (Response::isSuccess(resp))
  {
    startup.setMaxProgress(100);
  }
}

void reportError(byte errorCode, String log)
{
  ui.showError(errorCode);
  console.info(log);
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
    reportError(UI::ERROR_CODE_SENSOR, ("Failed to read ultrasonic sensor"));
    internet.sendLog(F("error"), F("Failed to read ultrasonic sensor"));
    return;
  }
  int lvl = helpers.distanceToLevel(distance);
  ui.showLevel(lvl);
  internet.sendLevel(lvl);
}

void internetErrorCallback(String command, byte type, String desc)
{
  reportError(UI::ERROR_CODE_HTTP, command + String(F(": ")) + desc);
  if (Command::equals(command, F("!level")))
  {
    internet.disconnect(nullptr);
    internet.connect(nullptr);
    check();
  }
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
  ui.setup();
  checkTimer.attach(check);
  checkTimer.start();
  internet.setup(internetErrorCallback);
  startup.setup(connectCallback);
}

void loop()
{
  btnCheck.tick();
  ui.tick();
  checkTimer.tick();
  internet.tick();
  startup.tick();
}

#endif
