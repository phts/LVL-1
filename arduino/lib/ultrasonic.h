#ifndef ultrasonic_h
#define ultrasonic_h

#include <TimerMs.h>
#include "sensor_lib_shim.h"
#include "settings.h"
#include "level.h"
#include "utils.h"
#include "console.h"

typedef void (*OnDistanceCallback)(bool success, Distance distance, Mode mode, Distance samples[], byte samples_len, byte iterations);

class Ultrasonic
{
public:
  static bool isFailed(Distance distance, byte iteration = 5)
  {
    return (iteration == 1 && distance < 10) || distance < 0 || distance > DISTANCE_FOR_EMPTY;
  }

  Ultrasonic(byte pinTrigger, byte pinEcho, byte pinPower) : _sensor(pinTrigger, pinEcho), _samplesTimer(ULTRASONIC_SAMPLES_INTERVAL)
  {
    _pinPower = pinPower;
  }

  void setup(OnDistanceCallback cb)
  {
    _onDistanceCallback = cb;
    pinMode(_pinPower, OUTPUT);
    this->off();
  }

  void tick()
  {
    if (!_samplesTimer.tick())
    {
      return;
    }
    _iteration++;
    if (_iteration == 0)
    {
      this->on();
      return;
    }

    if (_iteration > ULTRASONIC_MAX_TRIES)
    {
      _iteration = ULTRASONIC_MAX_TRIES;
      handleDistance(_samples, _samplesGathered);
      return;
    }
    Distance distance = _sensor.measureDistanceCm();
    if (isFailed(distance, _iteration))
    {
      console.debug(F("Ultrasonic"), F("[skip] sample="), distance);
      return;
    }
    console.debug(F("Ultrasonic"), F("sample="), distance);
    _samples[_samplesGathered] = distance;
    _samplesGathered++;
    if (_samplesGathered >= ULTRASONIC_MAX_SAMPLES)
    {
      handleDistance(_samples, _samplesGathered);
    }
  }

  bool requestDistance(Mode mode)
  {
    if (_samplesTimer.active())
    {
      console.debug(F("Ultrasonic"), F("already requested, request skipped"));
      return false;
    }
    _samplesTimer.restart();
    _samplesGathered = 0;
    _iteration = -1;
    _mode = mode;
    console.debug(F("Ultrasonic"), F("request distance"));
    return true;
  }

private:
  byte _pinPower;
  Sensor _sensor;
  TimerMs _samplesTimer;
  OnDistanceCallback _onDistanceCallback = nullptr;
  byte _samplesGathered = 0;
  Distance _samples[ULTRASONIC_MAX_SAMPLES];
  int8_t _iteration;
  Mode _mode;

  void handleDistance(Distance samples[], byte samples_len)
  {
    _samplesTimer.stop();
    this->off();
    if (samples_len < ULTRASONIC_MAX_SAMPLES)
    {
      _onDistanceCallback(false, 0, _mode, samples, samples_len, _iteration);
      return;
    }

    console.debug(F("Ultrasonic"), F("iterations:"), _iteration);
    console.debug(F("Ultrasonic"), F("samples:"), samples, samples_len);
    utils.sort(samples, samples_len);
    Distance *middleSamples = utils.subarray(samples, ULTRASONIC_TRIM_SAMPLES, samples_len - ULTRASONIC_TRIM_SAMPLES);
    int s = samples_len - ULTRASONIC_TRIM_SAMPLES * 2;
    console.debug(F("Ultrasonic"), F("middle samples:"), middleSamples, s);
    Distance res = utils.average(middleSamples, s);
    delete[] middleSamples;
    console.debug(F("Ultrasonic"), F("distance:"), res);
    _onDistanceCallback(true, res, _mode, samples, samples_len, _iteration);
  }

  void on()
  {
    console.debug(F("Ultrasonic"), F("power=on"));
    digitalWrite(_pinPower, HIGH);
  }

  void off()
  {
    console.debug(F("Ultrasonic"), F("power=off"));
    digitalWrite(_pinPower, LOW);
  }
};

#endif
