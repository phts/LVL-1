#ifndef transport_h
#define transport_h

#define TRANSPORT_SUCCESS_RESPONSE F("ok!")
#define TRANSPORT_FAILURE_RESPONSE F("fail!")

#include <SoftwareSerial.h>
#include <TimerMs.h>
#include "console.h"
#include "settings.h"

typedef void (*OnResponseCallback)(String response);
typedef void (*OnFailureCallback)(byte type, String desc);

class Transport
{
public:
  static const byte FAILURE_TYPE_RESPONSE = 1;
  static const byte FAILURE_TYPE_EXEC_TIMEOUT = 2;
  static const byte FAILURE_TYPE_RESP_TIMEOUT = 3;

  Transport(SoftwareSerial *_serial) : executionTimeoutTimer(TRANSPORT_TIMEOUT_EXECUTION, 0, 1),
                                       responseTimeoutTimer(TRANSPORT_TIMEOUT_RESPONSE, 0, 1)
  {
    serial = _serial;
  }

  void exec(String _command)
  {
    exec(_command, nullptr, nullptr);
  }
  void exec(String _command, OnResponseCallback _onResponse)
  {
    exec(_command, _onResponse, nullptr);
  }
  void exec(String _command, OnResponseCallback _onResponse, OnFailureCallback _onFail)
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
    executionTimeoutTimer.start();
    responseTimeoutTimer.start();
  }

  void execWithValue(String _command, String _value, OnResponseCallback _onResponse, OnFailureCallback _onFail)
  {
    exec(_command + String(F("=")) + _value, _onResponse, _onFail);
  }

  boolean isBusy()
  {
    return state;
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
  OnResponseCallback onResponse;
  OnFailureCallback onFail;
  SoftwareSerial *serial;
  TimerMs executionTimeoutTimer;
  TimerMs responseTimeoutTimer;

  void tickState()
  {
    if (executionTimeoutTimer.tick())
    {
      fail(FAILURE_TYPE_EXEC_TIMEOUT, String(F("Timed out during execution: ")) + command);
      return;
    }
    if (responseTimeoutTimer.tick())
    {
      fail(FAILURE_TYPE_RESP_TIMEOUT, String(F("Timed out response from command: ")) + command);
      return;
    }
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
        responseTimeoutTimer.stop();
        return;
      }
    }
    else if (state == STATE_HANDLING)
    {
      state = STATE_WAITING;
      responseTimeoutTimer.restart();
      console.debug(F("Transport:: >"), response);
      if (response == TRANSPORT_SUCCESS_RESPONSE)
      {
        finish();
      }
      else if (response.startsWith(TRANSPORT_FAILURE_RESPONSE))
      {
        fail(FAILURE_TYPE_RESPONSE, response.substring(String(TRANSPORT_FAILURE_RESPONSE).length() + 1));
      }
      if (*onResponse)
      {
        onResponse(response);
      }
      response = "";
    }
  }

  void finish()
  {
    console.debug(F("Transport:: finish"));
    state = STATE_READY;
    executionTimeoutTimer.stop();
    responseTimeoutTimer.stop();
  }

  void fail(byte type, String desc)
  {
    finish();
    console.debug(F("Transport:: fail"), String(type) + desc);
    state = STATE_READY;
    if (*onFail)
    {
      onFail(type, desc);
    }
  }
};

#endif
