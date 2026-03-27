# ESP32 Robotic Car with Articulated Claw

## Project Overview
This repository contains the firmware for an ESP32-based robotic car equipped with a 3D-printed articulated claw. The system is designed to integrate precise servo control, an onboard user interface, and modular hardware components.

## Current Status: Hardware Validation Phase
The project is currently in the hardware validation phase. The `main` branch contains isolated test scripts located in the `examples/` directory to verify component functionality before integrating them into the main vehicle logic.

### Active Test: `examples/claw_test`
This module tests the integration of the ESP32, an onboard ST7789 TFT display, and PWM servo control via the Serial Monitor.

## Hardware Setup & Pinout

### Microcontroller
* **Board:** IdeaSpark ESP32 1.14" V4.1
* **Display Controller:** ST7789 (Internal SPI)
* **Display Pins:** `CS: 15`, `DC: 2`, `RST: 4`, `BLK: 32`

### Actuators & Power
* **Servo Control Pin:** GPIO 26 (Isolated from internal display routing)
* **Power Delivery:** Servos are powered via an external 5V source (Buck converter or USB-C expansion board) to prevent microcontroller brownouts.
* **Critical Wiring:** A common ground (GND) must be established between the ESP32 and the external servo power supply for stable PWM signal transmission.

## Engineering Notes & Hardware Constraints

During the initial testing of the 3D-printed rack-and-pinion claw mechanism, the following hardware constraints were identified:

1.  **Servo Selection for Articulation:** The claw mechanism requires standard **180-degree positional servos** (e.g., standard SG92R). These provide the absolute positioning required to safely open and close the physical gear mechanism without over-torquing the 3D-printed parts.
2.  **Continuous Rotation Repurposing:** **360-degree continuous rotation servos** (often visually identical to standard 9g servos) lack internal positioning limits and operate on speed/direction logic. These cannot be used for the claw articulation but are ideal candidates for the robotic car's primary drive wheels (open-loop speed control).

## Build Instructions
This project is configured using PlatformIO. To compile the current hardware test:

1. Ensure the `platformio.ini` file specifies the test directory: `src_dir = examples/claw_test`
2. Connect the ESP32 and build/upload the project via the PlatformIO interface.
3. Open the Serial Monitor (115200 baud) and input an angle to actuate the servo and update the onboard display.