#ifndef helpers_h
#define helpers_h

#include "settings.h"
#include "ultrasonic.h"

class Helpers
{
public:
  float distanceToLevel(Distance distance)
  {
    if (distance >= DISTANCE_FOR_EMPTY)
    {
      return 0.0;
    }
    if (distance <= DISTANCE_FOR_FULL)
    {
      return 100.0;
    }
    float level = DISTANCE_FOR_EMPTY - distance;
    float height = DISTANCE_FOR_EMPTY - DISTANCE_FOR_FULL;
    return level / height * 100.0;
  }

  byte calcErrorRate(byte iterations)
  {
    return round(100.0 * (iterations - ULTRASONIC_MAX_SAMPLES) / (ULTRASONIC_MAX_TRIES - ULTRASONIC_MAX_SAMPLES + 1));
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
