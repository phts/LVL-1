#ifndef main_h
#define main_h

#include <EncButton.h>
#include <Response.h>
#include <TimerMs.h>
#include "config.h"
#include "settings.h"
#include "level.h"
#include "ultrasonic.h"
#include "ui.h"
#include "helpers.h"
#include "console.h"
#include "indicator.h"
#include "led.h"
#include "internet.h"
#include "startup.h"
#include "remote_control.h"

Button btnMeasure(Config::PIN_BTN_MEASURE);
Ultrasonic ultrasonic(Config::PIN_ULTRASONIC_SENSOR_TRIGGER, Config::PIN_ULTRASONIC_SENSOR_ECHO);
Led led(Config::PIN_LED);
Indicator indicator(&led, Config::PIN_METER);
UI ui(&indicator, LEVEL_WARNING);
Internet internet(Config::PIN_MODEM_RX, Config::PIN_MODEM_TX);
Startup startup(&ui, &internet);
RemoteControl remoteControl(&internet);

TimerMs measureTimer(MEASURE_INTERVAL);

void connectCallback(String resp)
{
  if (Response::equals(resp, F("progress!")))
  {
    int progress = Response::valueOf(resp).toInt();
    startup.setMaxProgress(progress);
    if (progress == 100)
    {
      internet.sendLog(F("info"), F("Connected to Wi-Fi"));
    }
  }
}

void measure(bool restartTimer = false, bool byTimer = false)
{
  if (startup.isStarting())
  {
    console.info(F("Not started yet. Measure skipped..."));
    return;
  }
  bool requested = ultrasonic.requestDistance(byTimer || restartTimer ? MODE_AUTO : MODE_MANUAL);
  if (!requested)
  {
    return;
  }
  ui.showBusy();
  if (restartTimer)
  {
    measureTimer.restart();
  }
  if (restartTimer && !byTimer)
  {
    internet.sendLog(F("info"), F("Measure timer restarted"));
  }
}

void startedCallback()
{
  console.debug(F("main"), F("started"));
  measureTimer.start();
  internet.sendLog(F("info"), F("Started"));
  measure(true, true);
}

void distanceCallback(bool success, Distance distance, Mode mode, Distance samples[], byte samples_len)
{
  internet.sendLog(F("debug"), helpers.arrayToString(samples, samples_len));
  if (!success)
  {
    ui.showError(UI::ERROR_CODE_SENSOR);
    console.info(F("Failed to read ultrasonic sensor"));
    internet.sendLog(F("fatal"), F("Failed to read ultrasonic sensor"));
    return;
  }
  internet.sendLog(F("debug"), String(F("Distance from ultrasonic sensor: ")) + distance);
  int lvl = helpers.distanceToLevel(distance);
  ui.showLevel(lvl);
  internet.sendLevel(lvl, mode);
}

void transportErrorCallback(String command, byte type, String desc)
{
  String msg;
  msg.concat(F("Failed to process the command \""));
  msg.concat(command);
  msg.concat(F("\": "));
  msg.concat(desc);
  console.info(msg);
  if (startup.isStarting())
  {
    startup.setMaxProgress(100);
    return;
  }
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
    internet.connect(connectCallback, nullptr);
  }

  String msg2;
  msg2.concat(msg);
  msg2.concat(F("\ntype="));
  msg2.concat(type);
  msg2.concat(F(", code="));
  msg2.concat(uiError);
  internet.sendLog(F("error"), msg2);
}

void remoteControlActionCallback(String resp)
{
  if (Response::equals(resp, F("id!")))
  {
    remoteControl.setNextId(Response::valueOf(resp));
  }
  else if (Response::equals(resp, F("action!")))
  {
    remoteControl.setNextAction(Response::valueOf(resp));
  }
  else if (Response::isSuccess(resp))
  {
    remoteControl.saveNext();
  }
}

void btnMeasureCallback()
{
  switch (btnMeasure.action())
  {
  case EB_CLICK:
    measure(false);
    break;
  case EB_HOLD:
    console.debug(F("main"), F("button long press"));
    measure(true);
    break;
  }
}

void setup()
{
  Serial.begin(SERIAL_PORT);
  btnMeasure.attach(btnMeasureCallback);
  ui.setup();
  internet.setup(transportErrorCallback);
  ultrasonic.setup(distanceCallback);
  startup.setup(connectCallback, startedCallback);
  remoteControl.setup(remoteControlActionCallback);
}

void loop()
{
  btnMeasure.tick();
  ui.tick();
  if (measureTimer.tick())
  {
    measure(true, true);
  }
  internet.tick();
  ultrasonic.tick();
  startup.tick();
  remoteControl.tick();
  switch (remoteControl.getAction())
  {
  case RemoteControl::ACTION_MEASURE:
    remoteControl.markAsProcessed();
    measure(false);
    break;
  case RemoteControl::ACTION_MEASURE_AND_RESET_TIMER:
    remoteControl.markAsProcessed();
    measure(true);
    break;
  default:
    break;
  }
}

#endif
