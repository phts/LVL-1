#ifndef internet_h
#define internet_h

#include <SoftwareSerial.h>
#include "console.h"
#include "settings.h"
#include "transport.h"

class Internet
{
public:
  Internet(byte pinRx, byte pinTx) : _softSerial(pinRx, pinTx), _transport(&_softSerial)
  {
  }

  setup(OnFailureCallback failCallback)
  {
    _softSerial.begin(SERIAL_PORT);
    _failCallback = failCallback;
  }

  connect(OnResponseCallback onResponse)
  {
    _transport.exec(F("!connect"), onResponse, _failCallback);
  }

  disconnect(OnResponseCallback onResponse)
  {
    _transport.exec(F("!disconnect"), onResponse);
  }

  tick()
  {
    _transport.tick();
  }

  sendLevel(int value)
  {
    _transport.execWithValue(F("!level"), String(value), nullptr, _failCallback);
  }

  sendLog(String severity, String message)
  {
    _transport.execWithValue(F("!log"), severity + F(",") + message, nullptr, _failCallback);
  }

private:
  SoftwareSerial _softSerial;
  Transport _transport;
  OnFailureCallback _failCallback = nullptr;
};

#endif
