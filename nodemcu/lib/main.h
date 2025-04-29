#ifndef main_h
#define main_h
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClient.h>
#include <Command.h>
#include <Response.h>
#include <UrlEncode.h>
#include "debug.h"
#include "settings.h"

WiFiClient client;
HTTPClient http;
int progress = 0;

int sendProgress(int value)
{
  Serial.println(Response::withValue(F("progress!"), String(value)));
  return value;
}

void sendOk()
{
  Serial.println(Response::Success);
}

void sendFail(String desc)
{
  Serial.println(Response::withValue(Response::Failure, desc));
}

void connect()
{
  byte retry = 0;
  progress = sendProgress(0);
  debug(F("Connecting to"), WIFI_NAME);
  WiFi.begin(WIFI_NAME, WIFI_PASSWORD);
  progress = sendProgress(10);

  while (WiFi.status() != WL_CONNECTED && retry < WIFI_STATUS_MAX_RETRIES)
  {
    delay(WIFI_STATUS_INTERVAL);
    progress = sendProgress((70 - progress) / 2 + progress);
    retry++;
  }
  if (WiFi.status() != WL_CONNECTED)
  {
    sendFail(F("Not connected"));
    return;
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

  String url = String(LEVEL_POST_ENDPOINT);
  url.replace(F("{{value}}"), value);
  debug(url);
  http.begin(client, url);
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

void log(String value)
{
  if (WiFi.status() != WL_CONNECTED)
  {
    sendFail(F("Not connected"));
    return;
  }

  int comma = value.indexOf(',');
  if (comma < 0)
  {
    sendFail(F("Value wrong format: missing comma"));
    return;
  }
  String severity = value.substring(0, comma);
  String message = value.substring(comma + 1);

  String url = String(LOG_POST_ENDPOINT);
  url.replace(F("{{severity}}"), severity);
  url.replace(F("{{message}}"), urlEncode(message));
  debug(url);
  http.begin(client, url);
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
  if (Command::equals(cmd, F("!connect")))
  {
    connect();
  }
  else if (Command::equals(cmd, F("!disconnect")))
  {
    WiFi.disconnect();
    sendOk();
  }
  else if (Command::equals(cmd, F("!level")))
  {
    level(Command::valueOf(cmd));
  }
  else if (Command::equals(cmd, F("!log")))
  {
    log(Command::valueOf(cmd));
  }
  else if (Command::equals(cmd, F("!healthcheck")))
  {
    sendOk();
  }
  else if (Command::equals(cmd, F("!fail")))
  {
    sendFail(Command::valueOf(cmd));
  }
  else if (Command::equals(cmd, F("!ping")))
  {
    int times = Command::valueOf(cmd).toInt();
    for (int i = 0; i < times; i++)
    {
      Serial.println(Command::withValue(F("pong!"), String(i + 1)));
      delay(10000);
    }
    sendOk();
  }
  else
  {
    sendFail(F("Unknown command"));
  }
}

#endif
