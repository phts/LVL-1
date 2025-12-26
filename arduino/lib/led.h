#ifndef led_h
#define led_h

class Led
{
public:
  Led(byte pin)
  {
    _pin = pin;
  }

  void setup()
  {
    pinMode(_pin, OUTPUT);
    write(LOW);
  }

  void write(byte val)
  {
    _status = val;
    digitalWrite(_pin, _disabled ? LOW : val);
  }

  void toggle()
  {
    write(_status == LOW ? HIGH : LOW);
  }

  void enable()
  {
    _disabled = false;
    digitalWrite(_pin, _status);
  }

  void disable()
  {
    _disabled = true;
    digitalWrite(_pin, LOW);
  }

private:
  byte _pin;
  byte _status;
  byte _disabled;
};

#endif
