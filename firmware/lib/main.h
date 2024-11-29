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

Button btnCheck(Config::PIN_BTN_CHECK);
Ultrasonic ultrasonic(Config::PIN_ULTRASONIC_SENSOR_TRIGGER, Config::PIN_ULTRASONIC_SENSOR_ECHO);
Led warningLed(Config::PIN_LED_WARNING);
Indicator indicator(&warningLed, Config::PIN_INDICATOR);
Level level(&indicator, LEVEL_WARNING);

TimerMs mainTimer(CHECK_INTERVAL);

void progressCallback(String resp)
{
  if (resp.startsWith(F("progress=")))
  {
    indicator.setLevel(resp.substring(9).toInt());
  }
  if (resp == TRANSPORT_SUCCESS_RESPONSE)
  {
    indicator.setLevel(100);
    indicator.setWarning(Indicator::WARN_OFF);
  }
}
Wifi wifi(&indicator, Config::PIN_WIFI_RX, Config::PIN_WIFI_TX, progressCallback);

void check()
{
  float distance = ultrasonic.getDistance();
  if (distance < 0)
  {
    console.log(F("Failed to read ultrasonic sensor"));
    indicator.setWarning(Indicator::WARN_GENERIC_ERROR);
    return;
  }
  console.log(F("Distance: "), distance);
  level.setValue(helpers.distanceToLevel(distance));
}

void btnCheckCallback()
{
  switch (btnCheck.action())
  {
  case EB_PRESS:
    check();
    mainTimer.start();
  }
}

void setup()
{
  Serial.begin(SERIAL_PORT);
  btnCheck.attach(btnCheckCallback);
  level.setup();
  mainTimer.start();
  wifi.setup();
}

void loop()
{
  btnCheck.tick();
  indicator.tick();
  if (mainTimer.tick())
  {
    check();
  }
  wifi.tick();
}

#endif
