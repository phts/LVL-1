#ifndef commands_rc_h
#define commands_rc_h

#include <ESP8266HTTPClient.h>
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include "../settings.h"
#include "../debug.h"
#include "../helpers.h"

void rc()
{
  if (WiFi.status() != WL_CONNECTED)
  {
    sendFail(String(F("Not connected: status=")) + WiFi.status());
    return;
  }

  debug(RC_GET_ENDPOINT);

  WiFiClient client;
  HTTPClient http;
  http.begin(client, RC_GET_ENDPOINT);

  int code = http.GET();
  debug(F("Response code"), code);
  if (code < 200 || code > 299)
  {
    sendFail(String(F("Response code: ")) + code);
    http.end();
    return;
  }

  String response = http.getString();
  http.end();
  debug(F("Response"), response);

  int actionDivider = response.indexOf('|');
  if (actionDivider < 0)
  {
    sendFail(F("Response wrong format: missing divider"));
    return;
  }
  String id = response.substring(0, actionDivider);
  String action = response.substring(actionDivider + 1);
  sendValue(F("id!"), id);

  int payloadDivider = action.indexOf('=');
  if (payloadDivider > 0)
  {
    sendValue(F("action!"), action.substring(0, payloadDivider));
    sendValue(F("payload!"), action.substring(payloadDivider + 1));
  }
  else
  {
    sendValue(F("action!"), action);
  }
  sendOk();
}

#endif
