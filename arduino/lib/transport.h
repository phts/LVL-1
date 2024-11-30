#ifndef transport_h
#define transport_h

#define TRANSPORT_SUCCESS_RESPONSE F("ok!")
#define TRANSPORT_FAILURE_RESPONSE F("fail!")

#include <SoftwareSerial.h>
#include "console.h"
#include "settings.h"

class Transport
{
public:
  static const byte FAILURE_TYPE_RESPONSE = 1;

  Transport(SoftwareSerial *_serial)
  {
    serial = _serial;
  }

  void exec(String _command)
  {
    exec(_command, nullptr, nullptr);
  }
  void exec(String _command, void (*_onResponse)(String))
  {
    exec(_command, _onResponse, nullptr);
  }
  bool exec(String _command, void (*_onResponse)(String), void (*_onFail)(byte, String))
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
    onFail = _onFail;
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
  void (*onFail)(byte type, String desc);
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
      else if (response.startsWith(TRANSPORT_FAILURE_RESPONSE))
      {
        abort();
        if (*onFail)
        {
          onFail(FAILURE_TYPE_RESPONSE, response.substring(String(TRANSPORT_FAILURE_RESPONSE).length() + 1));
        }
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
