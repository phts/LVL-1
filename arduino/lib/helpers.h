#ifndef helpers_h
#define helpers_h

#include "settings.h"

class Helpers
{
public:
  int distanceToLevel(float distance)
  {
    return constrain(map(distance, DISTANCE_FOR_EMPTY, DISTANCE_FOR_FULL, 0, 100), 0, 100);
  }

  String arrayToString(float array[], byte len)
  {
    String str;
    for (byte i = 0; i < len; i++)
    {
      str.concat(array[i]);
      str.concat(F(", "));
    }
    return str;
  }
};

Helpers helpers;

#endif
