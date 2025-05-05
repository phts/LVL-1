#ifndef TransportCore_Command_h
#define TransportCore_Command_h

class Command
{
public:
  static const char ValueDivider = '=';

  static bool equals(String c1, String c2)
  {
    return c1.startsWith(c2);
  }

  static String valueOf(String command)
  {
    int pos = command.indexOf(ValueDivider);
    if (pos < 0)
    {
      return F("");
    }
    return command.substring(pos + 1);
  }

  static String withValue(String command, String value)
  {
    command.concat(ValueDivider);
    command.concat(value);
    return command;
  }
};

#endif
