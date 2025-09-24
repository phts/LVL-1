#ifndef commands_ping_h
#define commands_ping_h

#include "../helpers.h"

void ping(int times)
{
  for (int i = 0; i < times; i++)
  {
    sendValue(F("pong!"), i + 1);
    delay(10000);
  }
  sendOk();
}

#endif
