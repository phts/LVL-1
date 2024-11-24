#ifndef main_h
#define main_h
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClient.h>
#include "debug.h"
#include "settings.h"

WiFiClient client;
HTTPClient http;
int progress = 0;

int sendProgress(int value)
{
  Serial.println(F("progress=") + String(value));
  return value;
}

void sendOk()
{
  Serial.println(F("ok"));
}

void sendFail()
{
  Serial.println(F("fail"));
}

void start()
{
  progress = sendProgress(0);
  debug(F("Connecting to"), WIFI_NAME);
  WiFi.begin(WIFI_NAME, WIFI_PASSWORD);
  progress = sendProgress(10);

  while (WiFi.status() != WL_CONNECTED)
  {
    delay(WIFI_STATUS_INTERVAL);
    progress = sendProgress((70 - progress) / 2 + progress);
  }

  progress = sendProgress(100);
  sendOk();
}

void level(String value)
{
  if (WiFi.status() != WL_CONNECTED)
  {
    sendFail();
    return;
  }

  Serial.println(LEVEL_POST_ENDPOINT + value);
  http.begin(client, LEVEL_POST_ENDPOINT + value);
  int httpResponseCode = http.POST(F(""));
  debug(F("Response code"), httpResponseCode);

  if (httpResponseCode <= 0)
  {
    sendFail();
  }
  else
  {
    sendOk();
  }
  http.end();
}

void setup()
{
  Serial.begin(SERIAL_PORT);
}

void loop()
{
  Serial.println(F("Command>"));
  while (!Serial.available())
  {
  }

  String cmd = Serial.readString();
  cmd.trim();
  if (cmd == F("!start"))
  {
    start();
  }
  else if (cmd == F("!level"))
  {
    Serial.println(F("Level>"));
    while (!Serial.available())
    {
    }
    String value = Serial.readString();
    value.trim();
    level(value);
  }
  else
  {
    debug(F("Unknown"));
  }
}

#endif
