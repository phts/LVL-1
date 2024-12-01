#ifndef TransportCore_Response_h
#define TransportCore_Response_h

#include "Command.h"

class Response
{
public:
  static inline const char *Success = "ok!";
  static inline const char *Failure = "fail!";

  static bool isSuccess(String resp)
  {
    return resp == Response::Success;
  }
  static bool isFailure(String resp)
  {
    return resp.startsWith(Response::Failure);
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
