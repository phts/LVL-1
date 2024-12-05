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
    String command;
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
  }

  void exec(String command)
  {
    exec(command, nullptr, nullptr);
  }
  void exec(String command, OnResponseCallback onResponse)
  {
    exec(command, onResponse, nullptr);
  }
  void exec(String command, OnResponseCallback onResponse, OnFailureCallback onFailure)
  {
    console.debug(F("Transport:: exec:"), command);
    if (_queue.isFull())
    {
      console.info(F("Command queue is full"));
      return;
    }
    _queue.enqueue({command, onResponse, onFailure});
    execQueue();
  }

  void execWithValue(String command, String value, OnResponseCallback onResponse, OnFailureCallback onFailure)
  {
    exec(Command::withValue(command, value), onResponse, onFailure);
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
      console.debug(F("Transport:: execQueue: empty"));
      return;
    }
    if (isBusy())
    {
      console.debug(F("Transport:: execQueue: busy"));
      return;
    }
    _processingEntry = _queue.dequeue();
    console.debug(F("Transport:: execQueue: taking next command: "), _processingEntry.command);
    _state = STATE_WAITING;
    _response = F("");
    _serial->flush();
    _serial->println(_processingEntry.command);
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
      console.debug(F("Transport:: >"), _response);
      if (Response::isSuccess(_response))
      {
        finish();
      }
      else if (Response::isFailure(_response))
      {
        fail(FAILURE_TYPE_RESPONSE, Response::valueOf(_response));
      }
      if (*_processingEntry.onResponse)
      {
        _processingEntry.onResponse(_response);
      }
      _response = F("");
    }
  }

  void finish()
  {
    console.debug(F("Transport:: finish"));
    _state = STATE_READY;
    _executionTimeoutTimer.stop();
    _responseTimeoutTimer.stop();
    _processingEntry = CommandEntry();
    execQueue();
  }

  void fail(byte type, String desc)
  {
    console.debug(F("Transport:: fail"), String(type) + desc);
    if (*_processingEntry.onFailure)
    {
      _processingEntry.onFailure(_processingEntry.command, type, desc);
    }
    finish();
  }
};

#endif
