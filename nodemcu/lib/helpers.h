#ifndef helpers_h
#define helpers_h

#include <Response.h>

void sendValue(const __FlashStringHelper *cmd, int value)
{
  Serial.print(cmd);
  Serial.print(Command::ValueDivider);
  Serial.println(value);
}
void sendValue(const __FlashStringHelper *cmd, String value)
{
  Serial.print(cmd);
  Serial.print(Response::ValueDivider);
  Serial.println(value);
}
void sendValue(const __FlashStringHelper *cmd, const __FlashStringHelper *value)
{
  Serial.print(cmd);
  Serial.print(Response::ValueDivider);
  Serial.println(value);
}
void sendValue(const __FlashStringHelper *cmd, const __FlashStringHelper *m1, int m2)
{
  Serial.print(cmd);
  Serial.print(Response::ValueDivider);
  Serial.print(m1);
  Serial.println(m2);
}
void sendValue(const __FlashStringHelper *cmd, const __FlashStringHelper *m1, String m2)
{
  Serial.print(cmd);
  Serial.print(Response::ValueDivider);
  Serial.print(m1);
  Serial.println(m2);
}

int sendProgress(int value)
{
  sendValue(F("progress!"), value);
  return value;
}

void sendOk()
{
  Serial.println(Response::success());
}

void sendFail(const __FlashStringHelper *desc)
{
  sendValue(Response::failure(), desc);
}
void sendFail(String desc)
{
  sendValue(Response::failure(), desc);
}
void sendFail(const __FlashStringHelper *m1, int m2)
{
  sendValue(Response::failure(), m1, m2);
}
void sendFail(const __FlashStringHelper *m1, String m2)
{
  sendValue(Response::failure(), m1, m2);
}

#endif
