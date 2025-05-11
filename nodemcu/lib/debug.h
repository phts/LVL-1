#ifndef debug_h
#define debug_h

#include "settings.h"

#if DEBUG
void debug(byte number)
{
  Serial.println(number);
}

void debug(const __FlashStringHelper *str)
{
  Serial.println(str);
}
void debug(String str)
{
  Serial.println(str);
}

void debug(const __FlashStringHelper *label, String value)
{
  Serial.print(label);
  Serial.print(F(": "));
  Serial.println(value);
}

void debug(const __FlashStringHelper *label, int value)
{
  Serial.print(label);
  Serial.print(F(": "));
  Serial.println(value);
}
#else
void debug(byte number)
{
}
void debug(const __FlashStringHelper *str)
{
}
void debug(String str)
{
}
void debug(const __FlashStringHelper *label, String value)
{
}
void debug(const __FlashStringHelper *label, int value)
{
}
#endif

#endif
