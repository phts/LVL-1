#ifndef ultrasonic_h
#define ultrasonic_h

#include <HCSR04.h>
#include <TimerMs.h>
#include "settings.h"
#include "utils.h"
#include "console.h"

typedef float Distance;
typedef void (*OnDistanceCallback)(bool success, Distance distance, bool mode, Distance samples[], byte samples_len);

class Ultrasonic
{
public:
  static bool isFailed(Distance distance)
  {
    return distance < 5 ||
           distance > DISTANCE_FOR_EMPTY ||
           (distance > 60 && distance < 67); // random wall
  }

  Ultrasonic(byte pinTrigger, byte pinEcho) : _sensor(pinTrigger, pinEcho), _samplesTimer(ULTRASONIC_SAMPLES_INTERVAL)
  {
  }

  void setup(OnDistanceCallback cb)
  {
    _onDistanceCallback = cb;
  }

  void tick()
  {
    if (!_samplesTimer.tick())
    {
      return;
    }

    _samplesTry++;
    if (_samplesTry >= ULTRASONIC_MAX_TRIES)
    {
      _samplesTimer.stop();
      handleDistance(_samples, _samplesGathered);
    }
    Distance distance = _sensor.measureDistanceCm();
    console.debug(F("Ultrasonic"), F("sample="), distance);
    if (isFailed(distance))
    {
      return;
    }
    _samples[_samplesGathered] = distance;
    _samplesGathered++;
    if (_samplesGathered >= ULTRASONIC_MAX_SAMPLES)
    {
      _samplesTimer.stop();
      handleDistance(_samples, _samplesGathered);
    }
  }

  bool requestDistance(bool mode)
  {
    if (_samplesTimer.active())
    {
      console.debug(F("Ultrasonic"), F("already requested, request skipped"));
      return false;
    }
    _samplesTimer.restart();
    _samplesGathered = 0;
    _samplesTry = 0;
    _mode = mode;
    console.debug(F("Ultrasonic"), F("request distance"));
    return true;
  }

private:
  UltraSonicDistanceSensor _sensor;
  TimerMs _samplesTimer;
  OnDistanceCallback _onDistanceCallback = nullptr;
  byte _samplesGathered = 0;
  Distance _samples[ULTRASONIC_MAX_SAMPLES];
  byte _samplesTry = 0;
  bool _mode;

  void handleDistance(Distance samples[], byte samples_len)
  {
    if (samples_len < ULTRASONIC_MAX_SAMPLES)
    {
      _onDistanceCallback(false, 0, _mode, samples, samples_len);
      return;
    }

    console.debug(F("Ultrasonic"), F("samples:"), samples, samples_len);
    utils.sort(samples, samples_len);
    Distance *middleSamples = utils.subarray(samples, ULTRASONIC_TRIM_SAMPLES, samples_len - ULTRASONIC_TRIM_SAMPLES);
    int s = samples_len - ULTRASONIC_TRIM_SAMPLES * 2;
    console.debug(F("Ultrasonic"), F("middle samples:"), middleSamples, s);
    Distance res = utils.average(middleSamples, s);
    delete[] middleSamples;
    console.debug(F("Ultrasonic"), F("distance:"), res);
    _onDistanceCallback(true, res, _mode, samples, samples_len);
  }
};

#endif
