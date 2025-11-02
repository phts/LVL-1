#ifndef ajsr04m_clone_h
#define ajsr04m_clone_h

class AJSR04M_clone
{
public:
  AJSR04M_clone(byte triggerPin, byte echoPin)
  {
    this->triggerPin = triggerPin;
    this->echoPin = echoPin;
    pinMode(triggerPin, OUTPUT);
    pinMode(echoPin, INPUT);
  }

  float measureDistanceCm()
  {
    digitalWrite(this->triggerPin, LOW);
    delayMicroseconds(2);
    digitalWrite(this->triggerPin, HIGH);
    delayMicroseconds(250); // https://forum.arduino.cc/t/sr04m-2-doesnt-give-any-lecture/1341273/15
    digitalWrite(this->triggerPin, LOW);
    unsigned long duration = pulseIn(this->echoPin, HIGH);

    float distanceCm = (duration / 2) * 0.0343;
    if (distanceCm == 0 || distanceCm > 400)
    {
      return -1.0;
    }
    return distanceCm;
  }

private:
  byte triggerPin;
  byte echoPin;
};

#endif
