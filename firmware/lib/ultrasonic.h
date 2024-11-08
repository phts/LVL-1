#ifndef ultrasonic_h
#define ultrasonic_h

#include <HCSR04.h>

class Ultrasonic
{
public:
  Ultrasonic(byte pinTrigger, byte pinEcho) : sensor(pinTrigger, pinEcho)
  {
  }

  float getDistance()
  {
    return sensor.measureDistanceCm();
  }

private:
  UltraSonicDistanceSensor sensor;
};

#endif
