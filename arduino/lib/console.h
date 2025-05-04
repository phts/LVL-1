#ifndef console_h
#define console_h

class Console
{
public:
  void info(String str)
  {
    Serial.println(str);
  }
  void info(const __FlashStringHelper *str)
  {
    Serial.println(str);
  }
#if DEBUG
  void debug(const __FlashStringHelper *str)
  {
    Serial.println(str);
  }
  void debug(const __FlashStringHelper *scope, const __FlashStringHelper *str)
  {
    Serial.print(scope);
    Serial.print(F(":: "));
    Serial.println(str);
  }
  void debug(const __FlashStringHelper *scope, String str)
  {
    Serial.print(scope);
    Serial.print(F(":: "));
    Serial.println(str);
  }

  void debug(const __FlashStringHelper *scope, const __FlashStringHelper *label, String value)
  {
    Serial.print(scope);
    Serial.print(F(":: "));
    Serial.print(label);
    Serial.print(F(" "));
    Serial.println(value);
  }
  void debug(const __FlashStringHelper *scope, const __FlashStringHelper *label, int value)
  {
    Serial.print(scope);
    Serial.print(F(":: "));
    Serial.print(label);
    Serial.print(F(" "));
    Serial.println(value);
  }
  void debug(const __FlashStringHelper *scope, const __FlashStringHelper *label, float value)
  {
    Serial.print(scope);
    Serial.print(F(":: "));
    Serial.print(label);
    Serial.print(F(" "));
    Serial.println(value);
  }
  void debug(const __FlashStringHelper *scope, const __FlashStringHelper *label, float array[], int len)
  {
    Serial.print(scope);
    Serial.print(F(":: "));
    Serial.print(label);
    Serial.print(F(" "));
    for (byte i = 0; i < len; i++)
    {
      Serial.print(array[i]);
      Serial.print(F(", "));
    }
    Serial.println();
  }
#else
  void debug(const __FlashStringHelper *scope, const __FlashStringHelper *str)
  {
  }
  void debug(const __FlashStringHelper *scope, String str)
  {
  }
  void debug(const __FlashStringHelper *scope, const __FlashStringHelper *label, String value)
  {
  }
  void debug(const __FlashStringHelper *scope, const __FlashStringHelper *label, int value)
  {
  }
  void debug(const __FlashStringHelper *scope, const __FlashStringHelper *label, float value)
  {
  }
  void debug(const __FlashStringHelper *scope, const __FlashStringHelper *label, float array[], int len)
  {
  }
#endif
};

Console console;

#endif
