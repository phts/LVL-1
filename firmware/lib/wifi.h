#ifndef wifi_h
#define wifi_h

#include <SoftwareSerial.h>
#include "console.h"
#include "settings.h"
#include "transport.h"

class Wifi
{
public:
  Wifi(byte pinRx, byte pinTx) : softSerial(pinRx, pinTx), transport(&softSerial)
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
    transport.exec(String(F("!level=")) + String(value), [](String resp)
                   { console.log(F("!connect>>> "), resp); });
  }

private:
  SoftwareSerial softSerial;
  Transport transport;
  void (*connectCallback)(String);
};

#endif
