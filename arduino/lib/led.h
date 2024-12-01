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
    digitalWrite(_pin, val);
  }

  void toggle()
  {
    write(_status == LOW ? HIGH : LOW);
  }

private:
  byte _pin;
  byte _status;
};

#endif
