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

int sendProgress(int value)
{
  sendValue(F("progress!"), value);
  return value;
}

void sendOk()
{
  Serial.println(Response::success());
}

void sendFail(String desc)
{
  sendValue(Response::failure(), desc);
}

#endif
