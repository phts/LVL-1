#ifndef wifi_h
#define wifi_h

#include <SoftwareSerial.h>
#include <ESP8266WiFi.h>
#include "console.h"
#include "settings.h"
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
    char baud[7];
    sprintf(baud, "%d", SERIAL_PORT);
    indicator->setLevel(10);
    softSerial.begin(SERIAL_PORT);
    char cmd[80];
    strcpy(cmd, "AT+IPR=");
    strcat(cmd, baud);
    // if (send(cmd, "OK", 20))
    //   indicator->setLevel(30);
    // send("AT&W", "OK", 20);
    if (!send("AT", "OK", 100))
    {
      indicator->setLevel(100);
      console.log("Failed to connect WiFi module");
      delay(1000);
    }
    indicator->setLevel(10);
    if (send("AT+CWMODE=1", "OK", 100))
      indicator->setLevel(30);
    if (send("AT+CIPMODE=0", "OK", 100))
      indicator->setLevel(50);
    if (send("AT+CIPMUX=1", "OK", 100))
      indicator->setLevel(70);

    // Serial.println("Continue....");
  }

  tick()
  {
    if (softSerial.available())
      Serial.write(softSerial.read());
    if (Serial.available())
      softSerial.write(Serial.read());
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
    softSerial.println(command);
    delay(delayValue);
    Serial.println("delay");
    Serial.println(softSerial.available());
    int av = softSerial.available();
    for (int i = 0; i < av; i++)
    {
      Serial.print(char(softSerial.read()));
    }
    Serial.println();
    return false;
    Serial.println(softSerial.find(expected));
    Serial.println(softSerial.find(expected));
    return softSerial.available() && softSerial.find(expected);
  }
};

#endif
