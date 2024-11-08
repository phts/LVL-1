#ifndef level_h
#define level_h

class Level
{
public:
  Level(byte _pinLedWarning, float _warningLevel)
  {
    pinLedWarning = _pinLedWarning;
    warningLevel = _warningLevel;
  }

  void setup()
  {
    pinMode(pinLedWarning, OUTPUT);
  }

  void setValue(float _value)
  {
    float value = _value;
    bool warning = value < warningLevel;
    digitalWrite(pinLedWarning, warning ? HIGH : LOW);
    if (value < 0)
    {
      Serial.println('Failed to read ultrasonic sensor');
    }
    else
    {
      Serial.println(value);
    }
  }

private:
  byte pinLedWarning;
  float warningLevel;
};

#endif
