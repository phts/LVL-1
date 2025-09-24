#ifndef commands_level_h
#define commands_level_h

#include <ESP8266HTTPClient.h>
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include "../settings.h"
#include "../debug.h"
#include "../helpers.h"

void level(String value, bool isManual = false)
{
  if (WiFi.status() != WL_CONNECTED)
  {
    sendFail(F("Not connected: status="), WiFi.status());
    return;
  }

  String url = String(LEVEL_POST_ENDPOINT);
  url.replace(F("{{value}}"), value);
  url.replace(F("{{mode}}"), isManual ? F("manual") : F("auto"));
  debug(url);

  WiFiClient client;
  HTTPClient http;
  http.begin(client, url);
  int code = http.POST(F(""));
  http.end();
  debug(F("Response code"), code);

  if (code < 200 || code > 299)
  {
    sendFail(F("Response code: "), code);
    return;
  }
  sendOk();
}

#endif
