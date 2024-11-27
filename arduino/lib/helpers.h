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
};

Helpers helpers;

#endif
