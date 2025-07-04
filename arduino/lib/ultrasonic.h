#ifndef ultrasonic_h
#define ultrasonic_h

#include <HCSR04.h>
#include <TimerMs.h>
#include "settings.h"
#include "utils.h"
#include "console.h"

typedef void (*OnDistanceCallback)(float distance, bool mode, float samples[], byte samples_len);

class Ultrasonic
{
public:
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
    float distance = _sensor.measureDistanceCm();
    console.debug(F("Ultrasonic"), F("sample="), distance);
    if (distance < 0)
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
  float _samples[ULTRASONIC_MAX_SAMPLES];
  byte _samplesTry = 0;
  bool _mode;

  void handleDistance(float samples[], byte samples_len)
  {
    if (samples_len < ULTRASONIC_MAX_SAMPLES)
    {
      console.debug(F("Ultrasonic"), F("distance:"), -1);
      _onDistanceCallback(-1, _mode, samples, samples_len);
      return;
    }

    utils.sort(samples, samples_len);
    console.debug(F("Ultrasonic"), F("samples:"), samples, samples_len);
    float *middleSamples = utils.subarray(samples, ULTRASONIC_TRIM_SAMPLES, samples_len - ULTRASONIC_TRIM_SAMPLES);
    int s = samples_len - ULTRASONIC_TRIM_SAMPLES * 2;
    console.debug(F("Ultrasonic"), F("middle samples:"), middleSamples, s);
    float res = utils.average(middleSamples, s);
    delete[] middleSamples;
    console.debug(F("Ultrasonic"), F("distance:"), res);
    _onDistanceCallback(res, _mode, samples, samples_len);
  }
};

#endif
