#ifndef internet_h
#define internet_h

#include <SoftwareSerial.h>
#include "console.h"
#include "settings.h"
#include "transport.h"

class Internet
{
public:
  Internet(byte pinRx, byte pinTx) : softSerial(pinRx, pinTx), transport(&softSerial)
  {
  }

  setup(OnFailureCallback _failCallback)
  {
    softSerial.begin(SERIAL_PORT);
    failCallback = _failCallback;
  }

  connect(OnResponseCallback onResponse)
  {
    transport.exec(F("!connect"), onResponse, failCallback);
  }

  tick()
  {
    transport.tick();
  }

  sendLevel(int value)
  {
    transport.exec(String(F("!level=")) + String(value), nullptr, failCallback);
  }

private:
  SoftwareSerial softSerial;
  Transport transport;
  OnFailureCallback failCallback = nullptr;
};

#endif
