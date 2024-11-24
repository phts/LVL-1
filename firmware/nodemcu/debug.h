#ifndef debug_h
#define debug_h

#include "settings.h"

#if DEBUG
void debug(byte number)
{
  Serial.println(String(number));
}

void debug(String str)
{
  Serial.println(str);
}

void debug(String label, String value)
{
  Serial.println(label + ": " + value);
}

void debug(String label, int value)
{
  Serial.println(label + ": " + String(value));
}
#else
void debug(byte str)
{
}
void debug(String str)
{
}
void debug(String label, String value)
{
}
void debug(String label, int value)
{
}
#endif

#endif
