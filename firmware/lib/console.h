#ifndef console_h
#define console_h

class Console
{
public:
  void log(String str)
  {
    Serial.println(str);
  }
  void log(String str, int value)
  {
    Serial.print(str);
    Serial.println(value);
  }
};

Console console;

#endif
