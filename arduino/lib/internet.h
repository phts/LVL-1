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

  setup(void (*_connectCallback)(String))
  {
    softSerial.begin(SERIAL_PORT);
    connectCallback = _connectCallback;
  }

  connect()
  {
    transport.exec(F("!connect"), connectCallback);
  }

  tick()
  {
    transport.tick();
  }

  sendLevel(int value)
  {
    transport.exec(String(F("!level=")) + String(value));
  }

private:
  SoftwareSerial softSerial;
  Transport transport;
  void (*connectCallback)(String) = nullptr;
};

#endif
