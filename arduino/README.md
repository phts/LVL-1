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
- [HC-SR04](https://github.com/d03n3rfr1tz3/HC-SR04)
- [TimerMs](https://github.com/GyverLibs/TimerMs)
