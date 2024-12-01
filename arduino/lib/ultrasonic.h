#ifndef ultrasonic_h
#define ultrasonic_h

#include <HCSR04.h>
#include "settings.h"
#include "utils.h"
#include "console.h"

class Ultrasonic
{
public:
  Ultrasonic(byte pinTrigger, byte pinEcho) : _sensor(pinTrigger, pinEcho)
  {
  }

  float getDistance()
  {
    float samples[ULTRASONIC_MAX_SAMPLES];
    byte realValues = 0;
    for (int i = 0; i < ULTRASONIC_MAX_TRIES; i++)
    {
      delay(ULTRASONIC_SAMPLES_INTERVAL);
      float distance = _sensor.measureDistanceCm();
      if (distance < 0)
      {
        continue;
      }
      samples[realValues] = distance;
      realValues++;
      if (realValues == ULTRASONIC_MAX_SAMPLES)
      {
        break;
      }
    }
    if (realValues < ULTRASONIC_MAX_SAMPLES)
    {
      console.debug(F("Ultrasonic:: distance:"), -1);
      return -1;
    }

    utils.sort(samples, ULTRASONIC_MAX_SAMPLES);
    console.debug(F("Ultrasonic:: samples:"), samples, ULTRASONIC_MAX_SAMPLES);
    float *middleSamples = utils.subarray(samples, ULTRASONIC_TRIM_SAMPLES, ULTRASONIC_MAX_SAMPLES - ULTRASONIC_TRIM_SAMPLES);
    int s = ULTRASONIC_MAX_SAMPLES - ULTRASONIC_TRIM_SAMPLES * 2;
    console.debug(F("Ultrasonic:: middle samples:"), middleSamples, s);
    float res = utils.average(middleSamples, s);
    delete[] middleSamples;
    console.debug(F("Ultrasonic:: distance:"), res);
    return res;
  }

private:
  UltraSonicDistanceSensor _sensor;
};

#endif
