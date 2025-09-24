#ifndef commands_connect_h
#define commands_connect_h

#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include "../settings.h"
#include "../debug.h"
#include "../helpers.h"

// WL_NO_SHIELD        = 255
// WL_IDLE_STATUS      = 0
// WL_NO_SSID_AVAIL    = 1
// WL_SCAN_COMPLETED   = 2
// WL_CONNECTED        = 3
// WL_CONNECT_FAILED   = 4
// WL_CONNECTION_LOST  = 5
// WL_WRONG_PASSWORD   = 6
// WL_DISCONNECTED     = 7

void connect()
{
  int progress = 0;
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
    sendFail(F("Not connected: status="), WiFi.status());
    return;
  }
  progress = sendProgress(100);
  sendOk();
}

#endif
