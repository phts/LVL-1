#ifndef led_h
#define led_h

class Led
{
public:
  Led(byte _pin)
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

#endif
