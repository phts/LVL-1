#ifndef main_h
#define main_h
#include <Command.h>
#include "settings.h"
#include "helpers.h"
#include "commands/connect.h"
#include "commands/disconnect.h"
#include "commands/fail.h"
#include "commands/healthcheck.h"
#include "commands/level.h"
#include "commands/log.h"
#include "commands/ping.h"
#include "commands/rc.h"

// HTTPC_ERROR_CONNECTION_FAILED   = -1
// HTTPC_ERROR_SEND_HEADER_FAILED  = -2
// HTTPC_ERROR_SEND_PAYLOAD_FAILED = -3
// HTTPC_ERROR_NOT_CONNECTED       = -4
// HTTPC_ERROR_CONNECTION_LOST     = -5
// HTTPC_ERROR_NO_STREAM           = -6
// HTTPC_ERROR_NO_HTTP_SERVER      = -7
// HTTPC_ERROR_TOO_LESS_RAM        = -8
// HTTPC_ERROR_ENCODING            = -9
// HTTPC_ERROR_STREAM_WRITE        = -10
// HTTPC_ERROR_READ_TIMEOUT        = -11

void setup()
{
  Serial.begin(SERIAL_PORT_BAUD);
}

void loop()
{
  if (!Serial.available())
  {
    return;
  }

  String cmd = Serial.readString();
  cmd.trim();
  debug(cmd);

  if (Command::equals(cmd, F("!connect")))
  {
    connect();
  }
  else if (Command::equals(cmd, F("!disconnect")))
  {
    disconnect();
  }
  else if (Command::equals(cmd, F("!level")))
  {
    level(Command::valueOf(cmd));
  }
  else if (Command::equals(cmd, F("!levelm")))
  {
    level(Command::valueOf(cmd), true);
  }
  else if (Command::equals(cmd, F("!log")))
  {
    log(Command::valueOf(cmd));
  }
  else if (Command::equals(cmd, F("!rc")))
  {
    rc();
  }
  else if (Command::equals(cmd, F("!healthcheck")))
  {
    healthcheck();
  }
  else if (Command::equals(cmd, F("!fail")))
  {
    fail(Command::valueOf(cmd));
  }
  else if (Command::equals(cmd, F("!ping")))
  {
    ping(Command::valueOf(cmd).toInt());
  }
  else
  {
    sendFail(F("Unknown command"));
  }
}

#endif
