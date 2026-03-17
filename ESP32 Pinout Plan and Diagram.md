# ESP32 Pinout Plan and Diagram for WATERFRONT Project

This document outlines the GPIO pin assignments for the ESP32 microcontroller in the WATERFRONT kayak rental system. The pinout is based on the configuration in `config.h` and supports up to 10 compartments. Pins are chosen to avoid conflicts with ESP32 reserved pins (e.g., flash, UART0).

## Pin Assignment Table

| GPIO Pin | Function | Compartment | Notes |
|----------|----------|-------------|-------|
| 0        | Reserved | N/A         | Avoid for user functions (boot pin) |
| 1        | UART0 TX | N/A         | Serial output |
| 2        | Reserved | N/A         | Avoid (boot pin) |
| 3        | UART0 RX | N/A         | Serial input |
| 4        | Reserved | N/A         | Avoid (boot pin) |
| 5        | Reserved | N/A         | Avoid (boot pin) |
| 6-11     | Flash    | N/A         | SPI flash pins |
| 12       | Servo Pin | Compartment 1 | LEDC channel 0 |
| 13       | Limit Open Pin | Compartment 1 | GPIO input, pull-up |
| 14       | Limit Close Pin | Compartment 1 | GPIO input, pull-up |
| 15       | Ultrasonic Trigger Pin | Compartment 1 | GPIO output |
| 16       | Ultrasonic Echo Pin | Compartment 1 | GPIO input |
| 17       | Weight Sensor Pin | Compartment 1 | GPIO input |
| 18       | Servo Pin | Compartment 2 | LEDC channel 1 |
| 19       | Limit Open Pin | Compartment 2 | GPIO input, pull-up |
| 20       | Limit Close Pin | Compartment 2 | GPIO input, pull-up |
| 21       | Ultrasonic Trigger Pin | Compartment 2 | GPIO output |
| 22       | Ultrasonic Echo Pin | Compartment 2 | GPIO input |
| 23       | Weight Sensor Pin | Compartment 2 | GPIO input |
| 24       | Servo Pin | Compartment 3 | LEDC channel 2 |
| 25       | Limit Open Pin | Compartment 3 | GPIO input, pull-up |
| 26       | Limit Close Pin | Compartment 3 | GPIO input, pull-up |
| 27       | Ultrasonic Trigger Pin | Compartment 3 | GPIO output |
| 28       | Ultrasonic Echo Pin | Compartment 3 | GPIO input |
| 29       | Weight Sensor Pin | Compartment 3 | GPIO input |
| 30       | Servo Pin | Compartment 4 | LEDC channel 3 |
| 31       | Limit Open Pin | Compartment 4 | GPIO input, pull-up |
| 32       | Limit Close Pin | Compartment 4 | GPIO input, pull-up |
| 33       | Ultrasonic Trigger Pin | Compartment 4 | GPIO output |
| 34       | Ultrasonic Echo Pin | Compartment 4 | GPIO input |
| 35       | Weight Sensor Pin | Compartment 4 | GPIO input |
| 36       | Servo Pin | Compartment 5 | LEDC channel 4 |
| 37       | Limit Open Pin | Compartment 5 | GPIO input, pull-up |
| 38       | Limit Close Pin | Compartment 5 | GPIO input, pull-up |
| 39       | Ultrasonic Trigger Pin | Compartment 5 | GPIO output |

## Diagram (ASCII Art)

```
ESP32-WROOM-32 Pinout for WATERFRONT

GND ---- GND
3V3 ---- 3.3V Supply
EN  ---- Enable (pull-up)
VP  ---- ADC2_0 (unused)
VN  ---- ADC2_3 (unused)
D34 ---- Ultrasonic Echo Pin (Comp 5)
D35 ---- Weight Sensor Pin (Comp 5)
D32 ---- Limit Close Pin (Comp 4)
D33 ---- Ultrasonic Trigger Pin (Comp 4)
D25 ---- Limit Close Pin (Comp 3)
D26 ---- Ultrasonic Trigger Pin (Comp 3)
D27 ---- Ultrasonic Echo Pin (Comp 3)
D14 ---- Limit Close Pin (Comp 1)
D12 ---- Servo Pin (Comp 1)
D13 ---- Limit Open Pin (Comp 1)
D15 ---- Ultrasonic Trigger Pin (Comp 1)
D2  ---- Reserved
D4  ---- Reserved
RX2 ---- UART2 RX (unused)
TX2 ---- UART2 TX (unused)
D5  ---- Reserved
D18 ---- Servo Pin (Comp 2)
D19 ---- Limit Open Pin (Comp 2)
D21 ---- Ultrasonic Trigger Pin (Comp 2)
D22 ---- Ultrasonic Echo Pin (Comp 2)
D23 ---- Weight Sensor Pin (Comp 2)
SDA ---- I2C SDA (unused)
SCL ---- I2C SCL (unused)
D17 ---- Ultrasonic Echo Pin (Comp 1)
D16 ---- Ultrasonic Trigger Pin (Comp 1)
D36 ---- Servo Pin (Comp 5)
D39 ---- Ultrasonic Trigger Pin (Comp 5)
D0  ---- Reserved
D1  ---- UART0 TX
D3  ---- UART0 RX
```

## Notes
- **LEDC Channels**: Used for servo control (50 Hz PWM). Channels 0-4 assigned to compartments 1-5.
- **GPIO Modes**: Limit switches are inputs with pull-up resistors. Ultrasonic sensors use output for trigger, input for echo.
- **Power Pins**: 3V3 for sensors, GND for common ground.
- **Expansion**: For more compartments, use additional pins (e.g., 40-49 if available).
- **Conflicts**: Avoid pins 0, 2, 4, 5, 6-11, 34-39 for ADC2 if WiFi is used.
- **Testing**: Verify with multimeter; ensure no shorts.

This pinout is configurable via `config.h`. Update as hardware changes.
