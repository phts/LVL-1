#ifndef commands_log_h
#define commands_log_h

#include <ESP8266HTTPClient.h>
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <UrlEncode.h>
#include "../settings.h"
#include "../debug.h"
#include "../helpers.h"

void log(String value)
{
  if (WiFi.status() != WL_CONNECTED)
  {
    sendFail(String(F("Not connected: status=")) + WiFi.status());
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

  WiFiClient client;
  HTTPClient http;
  http.begin(client, url);
  int code = http.POST(F(""));
  http.end();
  debug(F("Response code"), code);

  if (code < 200 || code > 299)
  {
    sendFail(String(F("Response code: ")) + code);
    return;
  }
  sendOk();
}

#endif
