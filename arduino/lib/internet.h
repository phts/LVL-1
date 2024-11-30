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

  setup(void (*_failCallback)(byte, String))
  {
    softSerial.begin(SERIAL_PORT);
    failCallback = _failCallback;
  }

  connect(void (*onResponse)(String))
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
  void (*failCallback)(byte, String) = nullptr;
};

#endif
