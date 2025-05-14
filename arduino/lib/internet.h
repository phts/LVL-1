#ifndef internet_h
#define internet_h

#include <SoftwareSerial.h>
#include "settings.h"
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

  void sendLevel(int value, bool isManual)
  {
    _transport.exec(isManual ? F("!levelm") : F("!level"), String(value), nullptr, _failCallback);
  }

  void sendLog(const __FlashStringHelper *severity, String message)
  {
    String msg;
    msg.concat(severity);
    msg.concat(F(","));
    msg.concat(message);
    _transport.exec(F("!log"), msg, nullptr, _failCallback);
  }

  void getRemoteControl(OnResponseCallback onResponse)
  {
    _transport.exec(F("!rc"), onResponse, _failCallback);
  }

private:
  SoftwareSerial _softSerial;
  Transport _transport;
  OnFailureCallback _failCallback = nullptr;
};

#endif
