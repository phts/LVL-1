#ifndef TransportCore_Response_h
#define TransportCore_Response_h

#include "Command.h"

class Response
{
public:
  static const char ValueDivider = Command::ValueDivider;

  static const __FlashStringHelper *success()
  {
    return F("ok!");
  }
  static const __FlashStringHelper *failure()
  {
    return F("fail!");
  }
  static bool isSuccess(String resp)
  {
    return resp == success();
  }
  static bool isFailure(String resp)
  {
    return resp.startsWith(failure());
  }
  static bool equals(String resp1, String resp2)
  {
    return Command::equals(resp1, resp2);
  }
  static String valueOf(String resp)
  {
    return Command::valueOf(resp);
  }
};

#endif
