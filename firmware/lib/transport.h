#ifndef transport_h
#define transport_h

#define TRANSPORT_SUCCESS_RESPONSE F("ok!")

#include <SoftwareSerial.h>
#include "console.h"

class Transport
{
public:
  Transport(SoftwareSerial *_serial)
  {
    serial = _serial;
  }

  void exec(String _command)
  {
    exec(_command, nullptr);
  }
  void exec(String _command, void (*_onResponse)(String))
  {
    console.debug(F("Transport:: exec:"), _command);

    if (executed)
    {
      console.info(F("Previous command still running:"), command);
      return;
    }
    executed = true;
    command = _command;
    onResponse = _onResponse;
    serial->println(command);
  }

  void abort()
  {
    console.debug(F("Transport:: abort"));
    executed = false;
  }

  void tick()
  {
    if (!executed)
    {
      return;
    }
    if (!serial->available())
    {
      return;
    }
    String response = "";
    while (serial->available())
    {
      char c = serial->read();
      response.concat(c);
      if (c == '\n')
      {
        break;
      }
      delay(10);
    }
    response.trim();
    console.debug(F("Transport:: >"), response);
    if (response == TRANSPORT_SUCCESS_RESPONSE)
    {
      abort();
    }
    if (*onResponse)
    {
      onResponse(response);
    }
  }

private:
  bool executed = false;
  String command;
  void (*onResponse)(String);
  SoftwareSerial *serial;
};

#endif
