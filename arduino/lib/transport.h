#ifndef transport_h
#define transport_h

#include <SoftwareSerial.h>
#include <ArduinoQueue.h>
#include <TimerMs.h>
#include <Command.h>
#include <Response.h>
#include "console.h"
#include "settings.h"

typedef void (*OnResponseCallback)(String response);
typedef void (*OnFailureCallback)(String command, byte type, String desc);

class Transport
{
public:
  struct CommandEntry
  {
    const __FlashStringHelper *command;
    String value;
    OnResponseCallback onResponse;
    OnFailureCallback onFailure;
  };
  static const byte FAILURE_TYPE_RESPONSE = 1;
  static const byte FAILURE_TYPE_EXEC_TIMEOUT = 2;
  static const byte FAILURE_TYPE_RESP_TIMEOUT = 3;

  Transport(SoftwareSerial *serial) : _executionTimeoutTimer(TRANSPORT_TIMEOUT_EXECUTION, 0, 1),
                                      _responseTimeoutTimer(TRANSPORT_TIMEOUT_RESPONSE, 0, 1),
                                      _queue(Transport::QUEUE_MAX_LEN)
  {
    _serial = serial;
    _response.reserve(100);
  }

  void exec(const __FlashStringHelper *command, OnResponseCallback onResponse = nullptr, OnFailureCallback onFailure = nullptr)
  {
    exec(command, F(""), onResponse, onFailure);
  }
  void exec(const __FlashStringHelper *command, String value, OnResponseCallback onResponse = nullptr, OnFailureCallback onFailure = nullptr)
  {
    console.debug(F("Transport"), F("exec:"), command);
    if (value.length() > 0)
    {
      console.debug(F("Transport"), F("with value:"), value);
    }
    if (_queue.isFull())
    {
      console.info(F("Command queue is full"));
      return;
    }
    _queue.enqueue({command, value, onResponse, onFailure});
    execQueue();
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
  static const byte QUEUE_MAX_LEN = 3;
  static const byte STATE_READY = 0;
  static const byte STATE_WAITING = 1;
  static const byte STATE_READING = 2;
  static const byte STATE_HANDLING = 3;

  byte _state = STATE_READY;
  String _response;
  SoftwareSerial *_serial;
  TimerMs _executionTimeoutTimer;
  TimerMs _responseTimeoutTimer;
  CommandEntry _processingEntry;
  ArduinoQueue<CommandEntry> _queue;

  void execQueue()
  {
    if (_queue.isEmpty())
    {
      console.debug(F("Transport"), F("execQueue: empty"));
      return;
    }
    if (isBusy())
    {
      console.debug(F("Transport"), F("execQueue: busy"));
      return;
    }
    _processingEntry = _queue.dequeue();
    console.debug(F("Transport"), F("execQueue: taking next command:"), _processingEntry.command);
    if (_processingEntry.value.length() > 0)
    {
      console.debug(F("Transport"), F("execQueue: with value:"), _processingEntry.value);
    }
    _state = STATE_WAITING;
    _response = F("");
    _serial->flush();
    if (_processingEntry.value.length() > 0)
    {
      _serial->print(_processingEntry.command);
      _serial->print(Command::ValueDivider);
      _serial->println(_processingEntry.value);
    }
    else
    {
      _serial->println(_processingEntry.command);
    }
    _executionTimeoutTimer.start();
    _responseTimeoutTimer.start();
  }

  void tickState()
  {
    if (_executionTimeoutTimer.tick())
    {
      fail(FAILURE_TYPE_EXEC_TIMEOUT, F("Timed out execution"));
      return;
    }
    if (_responseTimeoutTimer.tick())
    {
      fail(FAILURE_TYPE_RESP_TIMEOUT, F("Timed out response"));
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
      console.debug(F("Transport"), F(">"), _response);
      if (*_processingEntry.onResponse)
      {
        _processingEntry.onResponse(_response);
      }
      if (Response::isSuccess(_response))
      {
        finish();
      }
      else if (Response::isFailure(_response))
      {
        fail(FAILURE_TYPE_RESPONSE, Response::valueOf(_response));
      }
      _response = F("");
    }
  }

  void finish()
  {
    console.debug(F("Transport"), F("finish"));
    _state = STATE_READY;
    _executionTimeoutTimer.stop();
    _responseTimeoutTimer.stop();
    _processingEntry = CommandEntry();
    execQueue();
  }

  void fail(byte type, String desc)
  {
    console.debug(F("Transport"), F("fail: type="), type);
    console.debug(F("Transport"), F("fail: desc="), desc);
    if (*_processingEntry.onFailure)
    {
      _processingEntry.onFailure(_processingEntry.command, type, desc);
    }
    finish();
  }
};

#endif
