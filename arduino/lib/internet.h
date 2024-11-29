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

  setup()
  {
    softSerial.begin(SERIAL_PORT);
  }

  connect(void (*onResponse)(String))
  {
    transport.exec(F("!connect"), onResponse);
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
};

#endif
