# PHTS LVL-1

![](./doc/model.png)

Features:

- Use ultrasonic sensor to measure distance
- Show level on the big fancy indicator
- Send levels and logs to [szambo.tsaryk.com] via Wi-Fi
- 24/7 monitoring
- Remote control via internet

## Firmware

Consists of 2 modules: [Arduino Nano](./arduino) and [NodeMCU V3](./nodemcu).

## Hardware

- Arduino Nano<sup><span title="original or equivalent">\*</span></sup>
- NodeMCU V3<sup><span title="original or equivalent">\*</span></sup>
- Ultrasonic sensor: HC-SR04<sup><span title="original or equivalent">\*</span></sup> or AJ-SR04M<sup><span title="original or equivalent">\*</span></sup>
- Analog meter 0-5V
- Buttons/LEDs/etc

## Design

### Schematic diagram

Hosted on: [OSHWLab/phts/lvl-1](https://oshwlab.com/phts/lvl-1).

![](https://image-pro.easyeda.com/pullimages/0f48d2ae3bf24f1691df2f1edecb8f2f.webp)

### Deployment diagram

![](./doc/deployment.png)

[szambo.tsaryk.com]: https://github.com/phts/szambo.tsaryk.com

## Usage

### Indication

| State   | LED                                | Meter                                                               | Description                                                          |
| ------- | ---------------------------------- | ------------------------------------------------------------------- | -------------------------------------------------------------------- |
| Normal  | `______________` (constantly off)  | Actual level                                                        | Normally operational device                                          |
| Warning | `**************` (constantly on)   | Actual level                                                        | Level is reaching the maximum                                        |
| Busy    | `*______*______` (slowly blinking) | - Starting up: progress bar (number on `%` bar)<br>- Busy: not used | The device is starting up or performing an action                    |
| Failure | `*_*_*_*_*_*_*_` (fast blinking)   | Error code (number on `m³` bar)                                     | Operation failure. See the error code description in the table below |

#### Errors

| Code | Type      | Description                          |
| ---- | --------- | ------------------------------------ |
| 1    | Fatal     | Ultrasonic sensor failure            |
| 2    | Temporary | NodeMCU module communication failure |
| 3    | Temporary | Wi-Fi connection failure             |
| 4    | Temporary | Unexpected HTTP response             |

- `Fatal` &mdash; will be indicated constantly until the failure is gone
- `Temporary` &mdash; will be hidden after 1 minute and back to the previous state
