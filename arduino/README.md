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
- [NewPing](https://bitbucket.org/teckel12/arduino-new-ping/wiki/Home)
- [TimerMs](https://github.com/GyverLibs/TimerMs)
