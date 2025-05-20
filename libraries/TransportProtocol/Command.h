#ifndef TransportCore_Command_h
#define TransportCore_Command_h

class Command
{
public:
  static const char ValueDivider = '=';

  static bool equals(String str, const __FlashStringHelper *cmd)
  {
    int divider = str.indexOf(ValueDivider);
    if (divider < 0)
    {
      return str.equals(cmd);
    }
    return str.substring(0, divider).equals(cmd);
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
};

#endif
