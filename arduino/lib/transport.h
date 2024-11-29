#ifndef transport_h
#define transport_h

#define TRANSPORT_SUCCESS_RESPONSE F("ok!")

#include <SoftwareSerial.h>
#include "console.h"
#include "settings.h"

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
    if (state != STATE_READY)
    {
      console.info(F("Previous command still running:"), command);
      return;
    }
    state = STATE_WAITING;
    response = "";
    command = _command;
    onResponse = _onResponse;
    serial->flush();
    serial->println(command);
  }

  void abort()
  {
    console.debug(F("Transport:: abort"));
    state = STATE_READY;
  }

  void tick()
  {
    tickState();
  }

private:
  static const byte STATE_READY = 0;
  static const byte STATE_WAITING = 1;
  static const byte STATE_READING = 2;
  static const byte STATE_HANDLING = 3;

  byte state = STATE_READY;
  String command;
  String response;
  void (*onResponse)(String);
  SoftwareSerial *serial;

  void tickState()
  {
    if (state == STATE_READY)
    {
#if DEBUG_RXTX
      if (serial->available())
        Serial.write(serial->read());
      if (Serial.available())
        serial->write(Serial.read());
#endif
      return;
    }
    else if (state == STATE_WAITING)
    {
      if (serial->available())
      {
        state = STATE_READING;
      }
    }
    else if (state == STATE_READING)
    {
      if (!serial->available())
      {
        state = STATE_WAITING;
        return;
      }
      char c = serial->read();
      response.concat(c);
      if (c == '\n')
      {
        response.trim();
        state = STATE_HANDLING;
        return;
      }
    }
    else if (state == STATE_HANDLING)
    {
      state = STATE_WAITING;
      console.debug(F("Transport:: >"), response);
      if (response == TRANSPORT_SUCCESS_RESPONSE)
      {
        abort();
      }
      if (*onResponse)
      {
        onResponse(response);
      }
      response = "";
    }
  }
};

#endif
