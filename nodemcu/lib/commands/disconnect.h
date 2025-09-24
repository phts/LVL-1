#ifndef commands_disconnect_h
#define commands_disconnect_h

#include <ESP8266WiFi.h>
#include "../helpers.h"

void disconnect()
{
  WiFi.disconnect();
  sendOk();
}

#endif
