#ifndef wifi_h
#define wifi_h

#include <SoftwareSerial.h>
#include "console.h"
#include "settings.h"
#include "indicator.h"
#include "transport.h"

class Wifi
{
public:
  Wifi(Indicator *_indicator, byte pinRx, byte pinTx, void (*_progressCallback)(String)) : softSerial(pinRx, pinTx), transport(&softSerial)
  {
    indicator = _indicator;
    progressCallback = _progressCallback;
  }

  setup()
  {
    softSerial.begin(SERIAL_PORT);
    indicator->setLed(Indicator::LED_INFO);
    transport.exec(F("!connect"), progressCallback);
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
  Indicator *indicator;
  void (*progressCallback)(String);
};

#endif
