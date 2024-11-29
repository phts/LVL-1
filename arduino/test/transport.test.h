#ifndef transport_test_h
#define transport_test_h

#include "../lib/settings.h"
#define DEBUG true
#define DEBUG_RXTX false

#include <SoftwareSerial.h>
#include "../lib/config.h"
#include "../lib/transport.h"

SoftwareSerial softSerial(Config::PIN_MODEM_RX, Config::PIN_MODEM_TX);
Transport transport(&softSerial);

void setup()
{
  Serial.begin(SERIAL_PORT);
  softSerial.begin(SERIAL_PORT);
}

void onResponse(String resp)
{
  Serial.println(String(F("<<<")) + resp);
}
void onFail(byte type, String desc)
{
  Serial.println(String(F("<<<FAIL: ")) + String(type) + String(F(" - ")) + desc);
}

void loop()
{
  if (!transport.isBusy())
  {
    Serial.println(F("Command>"));
    while (!Serial.available())
    {
    }
    String cmd = Serial.readString();
    cmd.trim();
    Serial.println(String(F(">>>")) + cmd);
    transport.exec(cmd, onResponse, onFail);
  }
  transport.tick();
}

#endif
