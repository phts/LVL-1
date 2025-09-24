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

// HTTPC_ERROR_CONNECTION_FAILED   = -1
// HTTPC_ERROR_SEND_HEADER_FAILED  = -2
// HTTPC_ERROR_SEND_PAYLOAD_FAILED = -3
// HTTPC_ERROR_NOT_CONNECTED       = -4
// HTTPC_ERROR_CONNECTION_LOST     = -5
// HTTPC_ERROR_NO_STREAM           = -6
// HTTPC_ERROR_NO_HTTP_SERVER      = -7
// HTTPC_ERROR_TOO_LESS_RAM        = -8
// HTTPC_ERROR_ENCODING            = -9
// HTTPC_ERROR_STREAM_WRITE        = -10
// HTTPC_ERROR_READ_TIMEOUT        = -11

// WL_NO_SHIELD        = 255
// WL_IDLE_STATUS      = 0
// WL_NO_SSID_AVAIL    = 1
// WL_SCAN_COMPLETED   = 2
// WL_CONNECTED        = 3
// WL_CONNECT_FAILED   = 4
// WL_CONNECTION_LOST  = 5
// WL_WRONG_PASSWORD   = 6
// WL_DISCONNECTED     = 7

WiFiClient client;
HTTPClient http;
int progress = 0;

void sendValue(const __FlashStringHelper *cmd, int value)
{
  Serial.print(cmd);
  Serial.print(Command::ValueDivider);
  Serial.println(value);
}
void sendValue(const __FlashStringHelper *cmd, String value)
{
  Serial.print(cmd);
  Serial.print(Response::ValueDivider);
  Serial.println(value);
}

int sendProgress(int value)
{
  sendValue(F("progress!"), value);
  return value;
}

void sendOk()
{
  Serial.println(Response::success());
}

void sendFail(String desc)
{
  sendValue(Response::failure(), desc);
}

void connect()
{
  WiFi.disconnect();
  delay(1000);
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
  delay(WIFI_STATUS_INTERVAL * 2);
  if (WiFi.status() != WL_CONNECTED)
  {
    sendFail(String(F("Not connected: status=")) + WiFi.status());
    return;
  }
  progress = sendProgress(100);
  sendOk();
}

void level(String value, bool isManual = false)
{
  if (WiFi.status() != WL_CONNECTED)
  {
    sendFail(String(F("Not connected: status=")) + WiFi.status());
    return;
  }

  String url = String(LEVEL_POST_ENDPOINT);
  url.replace(F("{{value}}"), value);
  url.replace(F("{{mode}}"), isManual ? F("manual") : F("auto"));
  debug(url);
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

void rc()
{
  if (WiFi.status() != WL_CONNECTED)
  {
    sendFail(String(F("Not connected: status=")) + WiFi.status());
    return;
  }

  debug(RC_GET_ENDPOINT);
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

void setup()
{
  Serial.begin(SERIAL_PORT_BAUD);
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
  else if (Command::equals(cmd, F("!levelm")))
  {
    level(Command::valueOf(cmd), true);
  }
  else if (Command::equals(cmd, F("!log")))
  {
    log(Command::valueOf(cmd));
  }
  else if (Command::equals(cmd, F("!rc")))
  {
    rc();
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
      sendValue(F("pong!"), i + 1);
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
