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
  Serial.println(F("ok!"));
}

void sendFail(String desc)
{
  Serial.println(String(F("fail!=")) + desc);
}

void connect()
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
    sendFail(F("Not connected"));
    return;
  }

  Serial.println(LEVEL_POST_ENDPOINT + value);
  http.begin(client, LEVEL_POST_ENDPOINT + value);
  int httpResponseCode = http.POST(F(""));
  debug(F("Response code"), httpResponseCode);

  if (httpResponseCode < 200 || httpResponseCode > 299)
  {
    sendFail(String(F("Response code: ")) + String(httpResponseCode));
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
  if (!Serial.available())
  {
    return;
  }

  String cmd = Serial.readString();
  cmd.trim();
  debug(cmd);
  if (cmd == F("!connect"))
  {
    connect();
  }
  else if (cmd == F("!disconnect"))
  {
    WiFi.disconnect();
    sendOk();
  }
  else if (cmd.startsWith(F("!level=")))
  {
    String value = cmd.substring(7);
    level(value);
  }
  else if (cmd == F("!healthcheck"))
  {
    sendOk();
  }
  else if (cmd.startsWith(F("!fail=")))
  {
    String value = cmd.substring(6);
    sendFail(value);
  }
  else
  {
    debug(F("Unknown"));
  }
}

#endif
