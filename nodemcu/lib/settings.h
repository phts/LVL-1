#ifndef settings_h
#define settings_h

#define DEBUG false
#define SERIAL_PORT 9600

#define WIFI_STATUS_INTERVAL 500
#define WIFI_STATUS_MAX_RETRIES 60

#if __has_include("settings.local.h")
#include "settings.local.h"
#endif
#endif
