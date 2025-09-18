#ifndef config_h
#define config_h

class Config
{
public:
  static const byte PIN_ULTRASONIC_SENSOR_TRIGGER = 2;
  static const byte PIN_ULTRASONIC_SENSOR_ECHO = 3;
  static const byte PIN_ULTRASONIC_SENSOR_POWER = 4;
  static const byte PIN_METER = 6;
  static const byte PIN_LED = 7;
  static const byte PIN_BTN_MEASURE = 8;
  static const byte PIN_MODEM_RX = 10;
  static const byte PIN_MODEM_TX = 11;
};

#endif
