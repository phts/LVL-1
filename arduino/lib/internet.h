#ifndef internet_h
#define internet_h

#include <SoftwareSerial.h>
#include "settings.h"
#include "level.h"
#include "transport.h"

class Internet
{
public:
  Internet(byte pinRx, byte pinTx) : _softSerial(pinRx, pinTx), _transport(&_softSerial)
  {
  }

  void setup(OnFailureCallback failCallback)
  {
    _softSerial.begin(SERIAL_PORT);
    _failCallback = failCallback;
  }

  void connect(OnResponseCallback onResponse)
  {
    connect(onResponse, _failCallback);
  }
  void connect(OnResponseCallback onResponse, OnFailureCallback overridenFailCallback)
  {
    _transport.exec(F("!connect"), onResponse, overridenFailCallback);
  }

  void disconnect(OnResponseCallback onResponse)
  {
    _transport.exec(F("!disconnect"), onResponse);
  }

  void tick()
  {
    _transport.tick();
  }

  void sendLevel(int value, Mode mode)
  {
    _transport.exec(mode == MODE_MANUAL ? F("!levelm") : F("!level"), String(value), nullptr, _failCallback);
  }

  void sendLog(const __FlashStringHelper *severity, const __FlashStringHelper *message)
  {
    String arg;
    arg.concat(severity);
    arg.concat(F(","));
    arg.concat(message);
    sendLog(arg);
  }
  void sendLog(const __FlashStringHelper *severity, String message)
  {
    String arg;
    arg.concat(severity);
    arg.concat(F(","));
    arg.concat(message);
    sendLog(arg);
  }
  void sendLog(const __FlashStringHelper *severity, const __FlashStringHelper *m1, String m2, const __FlashStringHelper *m3, byte value)
  {
    String arg;
    arg.reserve(100);
    arg.concat(severity);
    arg.concat(F(","));
    arg.concat(m1);
    arg.concat(m2);
    arg.concat(m3);
    arg.concat(value);
    sendLog(arg);
  }
  void sendLog(const __FlashStringHelper *severity, const __FlashStringHelper *message, float value)
  {
    String arg;
    arg.concat(severity);
    arg.concat(F(","));
    arg.concat(message);
    arg.concat(value);
    sendLog(arg);
  }
  void sendLog(const __FlashStringHelper *severity, const __FlashStringHelper *m1, long value, const __FlashStringHelper *m2)
  {
    String arg;
    arg.reserve(40);
    arg.concat(severity);
    arg.concat(F(","));
    arg.concat(m1);
    arg.concat(value);
    arg.concat(m2);
    sendLog(arg);
  }

  void getRemoteControl(OnResponseCallback onResponse)
  {
    _transport.exec(F("!rc"), onResponse, _failCallback);
  }

private:
  SoftwareSerial _softSerial;
  Transport _transport;
  OnFailureCallback _failCallback = nullptr;

  void sendLog(String arg)
  {
    _transport.exec(F("!log"), arg, nullptr, _failCallback);
  }
};

#endif
