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

  float distanceToLevelM3(float distance)
  {
    if (distance >= DISTANCE_FOR_EMPTY)
    {
      return 0;
    }
    if (distance <= DISTANCE_FOR_FULL)
    {
      return LEVEL_MAX_M3;
    }
    float level = (float)DISTANCE_FOR_EMPTY - distance;
    float height = DISTANCE_FOR_EMPTY - DISTANCE_FOR_FULL;
    return level / height * LEVEL_MAX_M3;
  }

  String arrayToString(float array[], byte len)
  {
    if (len == 0)
    {
      return F("");
    }
    byte digits1 = 0;
    byte digits2 = 0;
    byte digits3 = 0;
    for (byte i = 0; i < len; i++)
    {
      if (array[i] < 10)
      {
        digits1++;
      }
      else if (array[i] < 100)
      {
        digits2++;
      }
      else
      {
        digits3++;
      }
    }

    String str;
    str.reserve(5 * digits1 + 6 * digits2 + 7 * digits3 - 1); // 1.34,12.45,123.56
    for (byte i = 0; i < len - 1; i++)
    {
      str.concat(array[i]);
      str.concat(F(","));
    }
    str.concat(array[len - 1]);
    return str;
  }
};

Helpers helpers;

#endif
