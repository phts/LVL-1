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

  void sendLevel(int value)
  {
    _transport.execWithValue(F("!level"), String(value), nullptr, _failCallback);
  }

  void sendLog(String severity, String message)
  {
    severity.concat(F(","));
    severity.concat(message);
    _transport.execWithValue(F("!log"), severity, nullptr, _failCallback);
  }

private:
  SoftwareSerial _softSerial;
  Transport _transport;
  OnFailureCallback _failCallback = nullptr;
};

#endif
