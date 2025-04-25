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

void check()
{
  if (startup.isStarting())
  {
    String msg = F("Not started yet. Check skipped...");
    console.info(msg);
    internet.sendLog(F("warn"), msg);
    return;
  }
  checkTimer.setTime(CHECK_INTERVAL);
  checkTimer.start();
  float distance = ultrasonic.getDistance();
  if (distance < 0)
  {
    ui.showError(UI::ERROR_CODE_SENSOR);
    console.info(F("Failed to read ultrasonic sensor"));
    internet.sendLog(F("fatal"), F("Failed to read ultrasonic sensor"));
    return;
  }
  int lvl = helpers.distanceToLevel(distance);
  ui.showLevel(lvl);
  internet.sendLevel(lvl);
}

void transportErrorCallback(String command, byte type, String desc)
{
  String msg = String(F("Failed to process the command \"")) + command + F("\": ") + desc;
  console.info(msg);
  if (Command::equals(command, F("!log")))
  {
    return;
  }

  byte uiError;
  if (type == Transport::FAILURE_TYPE_EXEC_TIMEOUT || type == Transport::FAILURE_TYPE_RESP_TIMEOUT)
  {
    uiError = UI::ERROR_CODE_TRANSPORT;
  }
  else if (type == Transport::FAILURE_TYPE_RESPONSE && desc == F("Not connected"))
  {
    uiError = UI::ERROR_CODE_WIFI;
  }
  else
  {
    uiError = UI::ERROR_CODE_HTTP;
  }
  ui.showError(uiError, true);
  if (Command::equals(command, F("!level")))
  {
    internet.disconnect(nullptr);
    internet.connect(nullptr, nullptr);
  }
  internet.sendLog(F("error"), msg + F("\ntype=") + type + F(", code=") + uiError);
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
  internet.setup(transportErrorCallback);
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
