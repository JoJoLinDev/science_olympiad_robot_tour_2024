https://github.com/user-attachments/assets/73d27fe7-5a3d-498a-9857-1317e4caa275

# Robot Tour Omniwheel Rover

An Arduino-based omniwheel rover for the Science Olympiad **Robot Tour** event, using four quadrature encoders and an MPU6050 IMU to drive straight and execute repeatable 90° turns on the official track.

## Overview

This project implements an autonomous robotic vehicle that follows a preprogrammed path on the Robot Tour track, targeting precise distances and right-angle movements. The robot uses encoder feedback on all four omniwheels plus optional MPU6050 heading stabilization to keep paths straight and turns consistent across runs.

## Features

- Four independently driven omniwheel motors with PWM control for forward, backward, left, and right motion.  
- Quadrature encoders on all four wheels with hardware interrupts for high-resolution position tracking.  
- Distance-based motion using a calibrated `encoderPerCm` factor to convert centimeters to encoder counts.  
- High-level motion functions:
  - `f(distance)` – move forward
  - `b(distance)` – move backward
  - `r(distance)` – move right
  - `l(distance)` – move left
- One-button start compatible with Robot Tour “ready-to-run” requirements.[file:16]

## Hardware

- **Microcontroller**: Arduino-compatible board that supports `analogWriteFreq(16000)`.  
- **Drive train**:
  - 4 DC motors with omniwheels  
  - Motor driver(s) for direction + PWM per wheel  
- **Sensors**:
  - 4 quadrature encoders (one per wheel)  
  - 1 MPU6050 IMU (gyroscope + accelerometer) for optional heading correction  
- **User input**:
  - Start button on digital pin 3 (configured with `INPUT_PULLUP`)  
- **Power**:
  - Battery pack meeting Robot Tour rules (up to eight AA cells, no lithium or lead-acid).[file:16]

## Pinout

```cpp
// Motor pins
const int wheel1_A_pin = 8;
const int wheel1_B_pin = 9;
const int wheel2_A_pin = 12;
const int wheel2_B_pin = 13;
const int wheel3_A_pin = 15;
const int wheel3_B_pin = 14;
const int whee



