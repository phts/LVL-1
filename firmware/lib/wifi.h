#ifndef wifi_h
#define wifi_h

#include <SoftwareSerial.h>
#include "indicator.h"

class Wifi
{
public:
  Wifi(Indicator *_indicator, byte pinRx, byte pinTx) : softSerial(pinRx, pinTx)
  {
    indicator = _indicator;
  }

  setup()
  {
    indicator->setLevel(10);
    // softSerial.begin(38400);
    while (!send("AT", "OK", 10000))
    {
      indicator->setLevel(100);
      delay(1000);
    }
    indicator->setLevel(10);
    if (send("AT+CWMODE=1", "OK", 10000))
      indicator->setLevel(30);
    if (send("AT+CIPMODE=0", "OK", 10000))
      indicator->setLevel(50);
    if (send("AT+CIPMUX=1", "OK", 10000))
      indicator->setLevel(70);

    // Serial.println("Continue....");
  }

  tick()
  {
    // if (softSerial.available())
    //   Serial.write(softSerial.read());
    // if (Serial.available())
    //   softSerial.write(Serial.read());
    // return;
    // while (softSerial.available() > 0)
    // {
    //   char a = softSerial.read();
    //   if (a == '\0')
    //     continue;
    //   if (a != '\r' && a != '\n' && (a < 32))
    //     continue;
    //   Serial.print(a);
    // }
    // while (Serial.available() > 0)
    // {
    //   char a = Serial.read();
    //   Serial.write(a);
    //   softSerial.write(a);
    // }
  }

private:
  SoftwareSerial softSerial;
  Indicator *indicator;

  bool send(String command, char *expected, int delayValue)
  {
    Serial.println(command);
    delay(delayValue);
    while (Serial.available() > 0)
    {
      if (Serial.find(expected))
      {
        return true;
      }
    }
    return false;
  }
};

#endif
