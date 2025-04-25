#ifndef console_h
#define console_h

class Console
{
public:
  void info(String str)
  {
    Serial.println(str);
  }
  void info(String str, String str2)
  {
    Serial.print(str + F(" "));
    Serial.println(str2);
  }
#if DEBUG
  void debug(String scope, String str)
  {
    Serial.print(scope);
    Serial.print(F(":: "));
    Serial.println(str);
  }
  void debug(String scope, String str, String str2)
  {
    Serial.print(scope);
    Serial.print(F(":: "));
    Serial.print(str + F(" "));
    Serial.println(str2);
  }
  void debug(String scope, String str, int value)
  {
    Serial.print(scope);
    Serial.print(F(":: "));
    Serial.print(str + F(" "));
    Serial.println(value);
  }
  void debug(String scope, String str, float value)
  {
    Serial.print(scope);
    Serial.print(F(":: "));
    Serial.print(str + F(" "));
    Serial.println(value);
  }
  void debug(String scope, String str, float array[], int len)
  {
    Serial.print(scope);
    Serial.print(F(":: "));
    Serial.print(str + F(" "));
    for (byte i = 0; i < len; i++)
    {
      Serial.print(array[i]);
      Serial.print(F(", "));
    }
    Serial.println();
  }
#else
  void debug(String scope, String str)
  {
  }
  void debug(String scope, String str, String str2)
  {
  }
  void debug(String scope, String str, int value)
  {
  }
  void debug(String scope, String str, float value)
  {
  }
  void debug(String scope, String str, float array[], int len)
  {
  }
#endif
};

Console console;

#endif
