# NodeMCU firmware

Responsible for internet connection and communication with the website API to store the data. Receives data from [the arduino module](../arduino).

## Communication

Via module's RX/TX pins and `SoftwareSerial` on consumer side.

```cpp
SoftwareSerial softSerial;
softSerial.begin(9600);
softSerial.println("!some_command");
```

## API

Usually a command looks like: `!<name>` or `!<name>=<params>`. And a response: `<name>!` or `<name>!=<value>`.

| Command                     | `ok!`                | `fail!=<Description...>`                                                      | Description                                                                |
| --------------------------- | -------------------- | ----------------------------------------------------------------------------- | -------------------------------------------------------------------------- |
| `!connect`                  | When connected       | -                                                                             | Connect to WiFi. During connection sends responds like `progress!=<value>` |
| `!disconnect`               | Always               |                                                                               | Disconnect WiFi                                                            |
| `!level=<value>`            | On 2XX HTTP response | - When not connected<br>- On non-2XX HTTP response                            | Send level value to the website                                            |
| `!log=<severity>,<message>` | On 2XX HTTP response | - When not connected<br>- On non-2XX HTTP response<br>- On value wrong format | Send log entry to the website                                              |
| `!healthcheck`              | Always               | -                                                                             | Respond with success at once                                               |
| `!fail=<desc>`              | -                    | Always                                                                        | Respond with failure at once (for testing)                                 |
| `!ping=<n>`                 | Always               |                                                                               | Send `pong!=<i>` back `n` times with 10 sec delay                          |

## Development

## Prepare

1. Add `http://arduino.esp8266.com/stable/package_esp8266com_index.json` into Preferences &rarr; `Additional Boards Manager URLs`
2. Go to `Boards Manager`
3. Install `esp8266`
4. Select `Tools` &rarr; `Board` &rarr; `ESP8266 Boards` &rarr; `NODEMCU 1.0 (ESP - 12E Module)`

### Libraries

- [UrlEncode](https://github.com/plageoj/urlencode)
