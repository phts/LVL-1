#ifndef transport_test_h
#define transport_test_h

#include "../lib/settings.h"
#undef DEBUG
#define DEBUG true
#undef DEBUG_RXTX
#define DEBUG_RXTX false

#include <SoftwareSerial.h>
#include "../lib/config.h"
#include "../lib/transport.h"

SoftwareSerial softSerial(Config::PIN_MODEM_RX, Config::PIN_MODEM_TX);
Transport transport(&softSerial);

void setup()
{
  Serial.begin(CONSOLE_SERIAL_PORT_BAUD);
  softSerial.begin(TRANSPORT_SERIAL_PORT_BAUD);
}

void onResponse(String resp)
{
  Serial.println(String(F("<<<")) + resp);
}
void onFail(String command, byte type, String desc)
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
    if (cmd == F("!connect"))
    {
      transport.exec(F("!connect"), onResponse, onFail);
    }
    else if (cmd == F("!disconnect"))
    {
      transport.exec(F("!disconnect"), onResponse, onFail);
    }
    else if (cmd.startsWith(F("!level=")))
    {
      transport.exec(F("!level"), cmd.substring(cmd.indexOf(F("=")) + 1), onResponse, onFail);
    }
    else if (cmd.startsWith(F("!levelm=")))
    {
      transport.exec(F("!levelm"), cmd.substring(cmd.indexOf(F("=")) + 1), onResponse, onFail);
    }
    else if (cmd.startsWith(F("!log=")))
    {
      transport.exec(F("!log"), cmd.substring(cmd.indexOf(F("=")) + 1), onResponse, onFail);
    }
    else if (cmd == F("!rc"))
    {
      transport.exec(F("!rc"), onResponse, onFail);
    }
    else if (cmd == F("!healthcheck"))
    {
      transport.exec(F("!healthcheck"), onResponse, onFail);
    }
    else if (cmd == F("!fail"))
    {
      transport.exec(F("!fail=reason1"), onResponse, onFail);
    }
    else if (cmd == F("!ping"))
    {
      transport.exec(F("!ping"), onResponse, onFail);
    }
    else
    {
      transport.exec(F("!unknown"), onResponse, onFail);
    }
  }
  transport.tick();
}

#endif
