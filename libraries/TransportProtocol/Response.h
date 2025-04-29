#ifndef TransportCore_Response_h
#define TransportCore_Response_h

#include "Command.h"

class Response
{
public:
  static String success()
  {
    return F("ok!");
  }
  static String failure()
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
  static String withValue(String resp, String value)
  {
    return Command::withValue(resp, value);
  }
};

#endif
