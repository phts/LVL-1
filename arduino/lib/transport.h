#ifndef transport_h
#define transport_h

#define TRANSPORT_SUCCESS_RESPONSE F("ok!")
#define TRANSPORT_FAILURE_RESPONSE F("fail!")

#include <SoftwareSerial.h>
#include <TimerMs.h>
#include "console.h"
#include "settings.h"

typedef void (*OnResponseCallback)(String response);
typedef void (*OnFailureCallback)(String command, byte type, String desc);

class Transport
{
public:
  static const byte FAILURE_TYPE_RESPONSE = 1;
  static const byte FAILURE_TYPE_EXEC_TIMEOUT = 2;
  static const byte FAILURE_TYPE_RESP_TIMEOUT = 3;

  Transport(SoftwareSerial *serial) : _executionTimeoutTimer(TRANSPORT_TIMEOUT_EXECUTION, 0, 1),
                                      _responseTimeoutTimer(TRANSPORT_TIMEOUT_RESPONSE, 0, 1)
  {
    _serial = serial;
  }

  void exec(String command)
  {
    exec(command, nullptr, nullptr);
  }
  void exec(String command, OnResponseCallback onResponse)
  {
    exec(command, onResponse, nullptr);
  }
  void exec(String command, OnResponseCallback onResponse, OnFailureCallback onFail)
  {
    console.debug(F("Transport:: exec:"), command);
    if (_state != STATE_READY)
    {
      console.info(F("Previous command still running:"), command);
      return;
    }
    _state = STATE_WAITING;
    _response = "";
    _command = command;
    _onResponse = onResponse;
    _onFail = onFail;
    _serial->flush();
    _serial->println(_command);
    _executionTimeoutTimer.start();
    _responseTimeoutTimer.start();
  }

  void execWithValue(String command, String value, OnResponseCallback onResponse, OnFailureCallback onFail)
  {
    exec(command + String(F("=")) + value, onResponse, onFail);
  }

  boolean isBusy()
  {
    return _state;
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

  byte _state = STATE_READY;
  String _command;
  String _response;
  OnResponseCallback _onResponse;
  OnFailureCallback _onFail;
  SoftwareSerial *_serial;
  TimerMs _executionTimeoutTimer;
  TimerMs _responseTimeoutTimer;

  void tickState()
  {
    if (_executionTimeoutTimer.tick())
    {
      fail(_command, FAILURE_TYPE_EXEC_TIMEOUT, F("Timed out execution"));
      return;
    }
    if (_responseTimeoutTimer.tick())
    {
      fail(_command, FAILURE_TYPE_RESP_TIMEOUT, F("Timed out response"));
      return;
    }
    if (_state == STATE_READY)
    {
#if DEBUG_RXTX
      if (_serial->available())
        Serial.write(_serial->read());
      if (Serial.available())
        _serial->write(Serial.read());
#endif
      return;
    }
    else if (_state == STATE_WAITING)
    {
      if (_serial->available())
      {
        _state = STATE_READING;
      }
    }
    else if (_state == STATE_READING)
    {
      if (!_serial->available())
      {
        _state = STATE_WAITING;
        return;
      }
      char c = _serial->read();
      _response.concat(c);
      if (c == '\n')
      {
        _response.trim();
        _state = STATE_HANDLING;
        _responseTimeoutTimer.stop();
        return;
      }
    }
    else if (_state == STATE_HANDLING)
    {
      _state = STATE_WAITING;
      _responseTimeoutTimer.restart();
      console.debug(F("Transport:: >"), _response);
      if (_response == TRANSPORT_SUCCESS_RESPONSE)
      {
        finish();
      }
      else if (_response.startsWith(TRANSPORT_FAILURE_RESPONSE))
      {
        fail(_command, FAILURE_TYPE_RESPONSE, _response.substring(String(TRANSPORT_FAILURE_RESPONSE).length() + 1));
      }
      if (*_onResponse)
      {
        _onResponse(_response);
      }
      _response = "";
    }
  }

  void finish()
  {
    console.debug(F("Transport:: finish"));
    _state = STATE_READY;
    _executionTimeoutTimer.stop();
    _responseTimeoutTimer.stop();
  }

  void fail(String command, byte type, String desc)
  {
    finish();
    console.debug(F("Transport:: fail"), String(type) + desc);
    _state = STATE_READY;
    if (*_onFail)
    {
      _onFail(command, type, desc);
    }
  }
};

#endif
