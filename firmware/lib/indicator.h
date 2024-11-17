#ifndef indicator_h
#define indicator_h

#include <TimerMs.h>
#include "console.h"

#define INDICATOR_WARN_OFF 0
#define INDICATOR_WARN_ON 1
#define INDICATOR_WARN_GENERIC_ERROR 2

#define SETTINGS_INDICATOR_ERROR_BLINK_INTERVAL 500

class WarningLed
{
public:
  WarningLed(byte _pin)
  {
    pin = _pin;
  }

  void setup()
  {
    pinMode(pin, OUTPUT);
    write(LOW);
  }

  void write(byte val)
  {
    status = val;
    digitalWrite(pin, val);
  }

  void toggle()
  {
    write(status == LOW ? HIGH : LOW);
  }

private:
  byte pin;
  byte status;
};

class Indicator
{
public:
  Indicator(byte _pinIndicator, byte _pinLedWarning) : warningLed(_pinLedWarning),
                                                       blinkTimer(SETTINGS_INDICATOR_ERROR_BLINK_INTERVAL)
  {
    pinIndicator = _pinIndicator;
  }

  void setup()
  {
    pinMode(pinIndicator, OUTPUT);
    warningLed.setup();
  }

  void tick()
  {
    if (blinkTimer.tick())
    {
      warningLed.toggle();
    }
  }

  void setLevel(byte value)
  {
    int pwm = constrain(map(value, 0, 100, 0, 255), 0, 255);
    analogWrite(pinIndicator, pwm);
    console.log(F("PWM: "), pwm);
  }

  void setWarning(byte type)
  {
    blinkTimer.stop();
    switch (type)
    {
    case INDICATOR_WARN_OFF:
      warningLed.write(LOW);
      break;
    case INDICATOR_WARN_ON:
      warningLed.write(HIGH);
      break;
    case INDICATOR_WARN_GENERIC_ERROR:
      blinkTimer.start();
      break;
    default:
      break;
    }
  }

private:
  WarningLed warningLed;
  byte pinIndicator;
  TimerMs blinkTimer;
};

#endif
