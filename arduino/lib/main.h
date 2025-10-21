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
Ultrasonic ultrasonic(Config::PIN_ULTRASONIC_SENSOR_TRIGGER, Config::PIN_ULTRASONIC_SENSOR_ECHO, Config::PIN_ULTRASONIC_SENSOR_POWER);
Led led(Config::PIN_LED);
Indicator indicator(&led, Config::PIN_METER);
UI ui(&indicator, LEVEL_WARNING);
Internet internet(Config::PIN_MODEM_RX, Config::PIN_MODEM_TX);
Startup startup(&ui, &internet);
RemoteControl remoteControl(&internet);

TimerMs measureTimer(MEASURE_INTERVAL);
TimerMs recoveryTimer(MEASURE_FATAL_RECOVERY_TIMER, false, 1);

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

void measure(bool restartTimer = false, bool byTimer = false, bool recovery = false)
{
  if (startup.isStarting())
  {
    console.info(F("Not started yet. Measure skipped..."));
    return;
  }
  byte mode = byTimer || restartTimer ? MODE_AUTO : (recovery ? MODE_RECOVERY : MODE_MANUAL);
  bool requested = ultrasonic.requestDistance(mode);
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

void distanceCallback(bool success, Distance distance, Mode mode, Distance samples[], byte samples_len, byte iterations)
{
  internet.sendLog(F("debug"), F("Samples: "), helpers.arrayToString(samples, samples_len), F("\nIterations: "), iterations);
  if (!success)
  {
    if (mode == MODE_AUTO)
    {
      recoveryTimer.restart();
    }
    ui.showError(UI::ERROR_CODE_SENSOR);
    console.info(F("Failed to read ultrasonic sensor"));
    internet.sendLog(F("fatal"), F("Failed to read ultrasonic sensor"));
    return;
  }
  internet.sendLog(F("debug"), F("Distance from ultrasonic sensor: "), distance);
  int lvl = helpers.distanceToLevel(distance);
  ui.showLevel(lvl);
  internet.sendLevel(lvl, mode);
}

void transportErrorCallback(String command, byte type, String desc)
{
  String msg;
  msg.reserve(60);
  msg.concat(F("Failed command \""));
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

  byte code;
  if (type == Transport::FAILURE_TYPE_EXEC_TIMEOUT || type == Transport::FAILURE_TYPE_RESP_TIMEOUT)
  {
    code = UI::ERROR_CODE_TRANSPORT;
  }
  else if (type == Transport::FAILURE_TYPE_RESPONSE && desc == F("Not connected"))
  {
    code = UI::ERROR_CODE_WIFI;
  }
  else
  {
    code = UI::ERROR_CODE_HTTP;
  }
  ui.showError(code, true);
  if (Command::equals(command, F("!level")))
  {
    internet.disconnect(nullptr);
    internet.connect(connectCallback, nullptr);
  }

  String log;
  log.reserve(75);
  log.concat(msg);
  log.concat(F("\ntype="));
  log.concat(type);
  log.concat(F(", code="));
  log.concat(code);
  internet.sendLog(F("error"), log);
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
  else if (Response::equals(resp, F("payload!")))
  {
    remoteControl.setNextActionPayload(Response::valueOf(resp));
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

void handleRemoteControl()
{
  byte action = remoteControl.getAction();
  if (action == RemoteControl::ACTION_NOTHING)
  {
    return;
  }
  remoteControl.markAsProcessed();
  switch (action)
  {
  case RemoteControl::ACTION_MEASURE:
    measure(false);
    break;
  case RemoteControl::ACTION_MEASURE_AND_RESET_TIMER:
    measure(true);
    break;
  case RemoteControl::ACTION_SET_MEASURE_INTERVAL:
    long time = remoteControl.getActionPayload().toInt() * 60 * 60 * 1000;
    if (time > 0)
    {
      measureTimer.setTime(time);
      internet.sendLog(F("info"), F("Measure interval updated: "), time, F(" ms"));
    }
    break;
  }
}

void setup()
{
  console.setup(CONSOLE_SERIAL_PORT_BAUD);
  console.info(F("PHTS LVL-1"));
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
  if (recoveryTimer.tick())
  {
    measure(false, false, true);
  }
  internet.tick();
  ultrasonic.tick();
  startup.tick();
  remoteControl.tick();
  handleRemoteControl();
}

#endif
