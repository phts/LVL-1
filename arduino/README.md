# Arduino firmware

The core logic which is responsible for calculating and displaying the data.

By design it is able to work without internet connection or even without whole [NodeMCU module](../nodemcu).

## Development

### Setup

1. Board: `Arduino Nano`
2. Processor: `ATmega328P`
3. Programmer: `AVRISP mkII`

### Libraries

- [ArduinoQueue](https://github.com/EinarArnason/ArduinoQueue)
- [EncButton](https://github.com/GyverLibs/EncButton)
- [HCSR04](https://github.com/Martinsos/arduino-lib-hc-sr04) (if such sensor is used)
- [TimerMs](https://github.com/GyverLibs/TimerMs)

### Emulator

Powered by [Wokwi](https://docs.wokwi.com/vscode/getting-started).

1. Update `diagram.json` [online](https://wokwi.com/projects/new/arduino-nano) if needed.
2. Arduino IDE → Export Compiled Binary
3. Trigger command `Wokwi: Start Simulator`
