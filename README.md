# ESP32 Robotic Arm with Barcode Scanning and Suction Control

This project controls a 3-axis robotic arm using an ESP32 microcontroller. The system supports manual control via keyboard or joystick, as well as automated block detection and retrieval using a barcode scanner. It also features a suction pump with solenoid valve control for precise object manipulation.

## Features

- Real-time manual control using WASD/QE keys or XYZ coordinates
- Barcode-based object detection and retrieval
- Suction pump and valve system for object pickup and release
- Predefined search grid for automated scanning
- Home positioning and section-based block drop-off
- Easy integration with voice commands via serial interface

## Hardware

- ESP32 microcontroller
- MAX robotic arm (custom build)
- Waveshare Barcode Scanner (UART)
- Suction pump and solenoid valves
- Power supply for arm and actuators

## Pin Configuration

| Function        | ESP32 Pin |
|----------------|-----------|
| Arm Motor X/Y/Z| 32, 33, 22, 23 |
| Pump Control   | 21        |
| Valve Control  | 19, 18, 5 |
| Barcode RX/TX  | 32 (RX), 33 (TX) |

## Getting Started

1. Clone this repository:
   ```bash
   git clone https://github.com/yourusername/esp32-robotic-arm.git
