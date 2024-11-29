#ifndef console_h
#define console_h

class Console
{
public:
  void log(String str)
  {
    Serial.println(str);
  }
  void log(String str, String str2)
  {
    Serial.print(str);
    Serial.println(str2);
  }
  void log(String str, int value)
  {
    Serial.print(str);
    Serial.println(value);
  }
  void log(String str, float value)
  {
    Serial.print(str);
    Serial.println(value);
  }
  void log(String str, float array[], int len)
  {
    Serial.print(str);
    for (byte i = 0; i < len; i++)
    {
      Serial.print(array[i]);
      Serial.print(F(", "));
    }
    Serial.println();
  }
};

Console console;

#endif
