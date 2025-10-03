#if defined(__AVR_ATmega328P__)
#if __has_include("arduino.local.h")
#include "arduino.local.h"
#else
#include "lib/main.h"
#endif
#endif
