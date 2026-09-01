# 🏆 Eviatech 2025 Robotics Suite

<div align="center">

[![Line Following Robot Demo](https://img.youtube.com/vi/o28n6NV4aHE/maxresdefault.jpg)](https://www.youtube.com/watch?v=o28n6NV4aHE)

### **Multi-Robot Autonomous & Teleoperated Platform for the Eviatech 2025 Robotics Competition**

[![PlatformIO](https://img.shields.io/badge/PlatformIO-VS%20Code-orange.svg?style=for-the-badge&logo=platformio)](https://platformio.org/)
[![Arduino Nano 33 BLE](https://img.shields.io/badge/Board-Arduino%20Nano%2033%20BLE-00979D.svg?style=for-the-badge&logo=arduino)](https://store.arduino.cc/products/arduino-nano-33-ble)
[![ESP32](https://img.shields.io/badge/Board-ESP32-red.svg?style=for-the-badge&logo=espressif)](https://www.espressif.com/)
[![Language](https://img.shields.io/badge/Language-C%2B%2B17-blue.svg?style=for-the-badge&logo=cplusplus)](https://isocpp.org/)
[![BLE Control](https://img.shields.io/badge/Control-Bluetooth%20LE%20%2F%20DashIO-purple.svg?style=for-the-badge)](https://dashio.io/guide-arduino-nano33ble/)
[![Author](https://img.shields.io/badge/Author-Tambu%20Precious%20Takum-0A66C2.svg?style=for-the-badge&logo=linkedin)](https://www.linkedin.com/in/tambu-precious-29bb67217/)
[![License](https://img.shields.io/badge/License-MIT-green.svg?style=for-the-badge)](LICENSE)

---

**A multi-disciplinary robotics codebase featuring autonomous combat sumo bots, surface water boats, high-speed line racers, and sensor nodes built for Eviatech 2025.**

[📹 Watch Line Follower YouTube Demo](https://www.youtube.com/watch?v=o28n6NV4aHE) • [📱 DashIO BLE Control Guide](https://dashio.io/guide-arduino-nano33ble/) • [👤 Author LinkedIn](https://www.linkedin.com/in/tambu-precious-29bb67217/)

</div>

---

## 📋 Table of Contents
- [👤 Author & Attribution](#-author--attribution)
- [✨ Project Overview](#-project-overview)
- [🤖 Competition Robot Modules](#-competition-robot-modules)
  - [1. 🥊 Sumo / Fight Robot (`Fight_Robot`)](#1--sumo--fight-robot-fight_robot)
  - [2. 🚤 Ingenuity Robot Boat (`Inginuity_Robot_boat`)](#2--ingenuity-robot-boat-inginuity_robot_boat)
  - [3. 🏎️ Race Robot / Line Follower (`Race Robot`)](#3--race-robot--line-follower-race-robot)
  - [4. 📡 Sensor Node Subsystem (`Sensor_node`)](#4--sensor-node-subsystem-sensor_node)
- [📱 Telemetry & DashIO App Control](#-telemetry--dashio-app-control)
- [⚙️ Hardware Requirements & Components](#️-hardware-requirements--components)
- [⚡ Compilation & Setup Guide](#-compilation--setup-guide)
- [🔗 Connected Projects](#-connected-projects)

---

## 👤 Author & Attribution

- **Creator & Lead Developer:** **Tambu Precious Takum**
- **LinkedIn Profile:** [linkedin.com/in/tambu-precious-29bb67217](https://www.linkedin.com/in/tambu-precious-29bb67217/)
- **Organization:** Hardware Innovation Valley Community [hwivc.org](https://hwivc.org)
- **Competition:** Eviatech 2025 Robotics Competition

---

## ✨ Project Overview

The **Eviatech 2025** repository contains the complete firmware suite for four distinct robotic systems built for the Eviatech 2025 competition challenges. 

Powered primarily by the **Arduino Nano 33 BLE** (with support for **ESP32 Wi-Fi** expansion), the platform utilizes **PlatformIO** for firmware compilation and integrates with the **DashIO** mobile interface to provide real-time telemetry, animated speed sliders, and wireless manual override controls over Bluetooth Low Energy (BLE) and Wi-Fi.

```
       ┌───────────────────────────────────────────────────────────┐
       │                EVIATECH 2025 ROBOTICS SUITE               │
       │                                                           │
       │  ┌──────────────┐  ┌──────────────────┐  ┌──────────────┐ │
       │  │ Fight Robot  │  │ Ingenuity Boat   │  │  Race Robot  │ │
       │  │ (Sumo Combat)│  │ (Water Surface)  │  │(Line Follow) │ │
       │  └──────┬───────┘  └────────┬─────────┘  └──────┬───────┘ │
       └─────────┼───────────────────┼───────────────────┼─────────┘
                 │                   │                   │
                 ▼                   ▼                   ▼
      ┌─────────────────────────────────────────────────────────────┐
      │       ARDUINO NANO 33 BLE / ESP32 DASHIO TELEMETRY          │
      │    Wireless Slider Motor Control & Autonomous Sensors       │
      └─────────────────────────────────────────────────────────────┘
```

---

## 🤖 Competition Robot Modules

### 1. 🥊 Sumo / Fight Robot (`Fight_Robot`)
Located in [`Fight_Robot/`](Fight_Robot/) and [`sumo_Robot.cpp`](sumo_Robot.cpp).
- **Function:** Autonomous combat sumo bot designed for arena survival and ring pushing.
- **Key Logic:** Combines ultrasonic distance sensing (`ULTRAsonic.h`) for enemy target acquisition with infrared edge detection (`IRSensor.h`) to prevent self-ringout.
- **Motor Control:** Dual-channel PWM differential drive with soft acceleration and sudden-stop counter-thrust routines (`motors.h`).

### 2. 🚤 Ingenuity Robot Boat (`Inginuity_Robot_boat`)
Located in [`Inginuity_Robot_boat/`](Inginuity_Robot_boat/).
- **Function:** Surface water vehicle engineered for aquatic maneuverability and obstacle navigation.
- **Key Logic:** Low-power motor control firmware built with custom `config.h` pinouts for moisture-resistant motor drivers.
- **Wireless Link:** Responsive BLE steering control using DashIO app sliders.

### 3. 🏎️ Race Robot / Line Follower (`Race Robot`)
Located in [`Race Robot/`](Race Robot/).
- **Function:** High-speed line-following and precision race bot.
- **Video Demo:** Watch the robot in action on [YouTube](https://www.youtube.com/watch?v=o28n6NV4aHE).
- **Key Logic:** Sensor array reading with PID control loop for smooth curve tracking and minimal overshoot. Includes real-time speed adjustments via wireless controls.

### 4. 📡 Sensor Node Subsystem (`Sensor_node`)
Located in [`Sensor_node/`](Sensor_node/).
- **Function:** Modular sensor integration layer providing reusable sensor drivers across all robots.
- **Components:**
  - `IRSensor.h`: Analog & digital IR reflectometer reading.
  - `ULTRAsonic.h`: Ultrasonic echo duration measurement and distance conversion.
  - `Sensors.h`: Unified sensor manager polling environment state.
  - `motors.h`: Standardized H-bridge motor driver interface.

---

## 📱 Telemetry & DashIO App Control

The robotics suite uses **DashIO** for real-time mobile dashboard control over Bluetooth Low Energy (BLE).

- **Dynamic Speed Sliders:** Built-in sliders allow real-time tuning of minimum/maximum PWM motor speed limits without re-flashing firmware.
- **Animated GUI Elements:** Custom animation functions send state updates back to the smartphone screen to visualize active throttle and sensor triggers.
- **Cross-Platform Compatibility:** Designed for **Arduino Nano 33 BLE** and expandable to **ESP32** over Wi-Fi TCP/UDP sockets.
- **Resource Guide:** Learn more at the official [DashIO Arduino Nano 33 BLE Guide](https://dashio.io/guide-arduino-nano33ble/).

---

## ⚙️ Hardware Requirements & Components

| Component | Quantity | Description |
| :--- | :---: | :--- |
| **Arduino Nano 33 BLE** | 1+ | Main competition microcontroller with onboard nRF52840 & Bluetooth LE |
| **ESP32 DevKit V1** | 1+ | Optional Wi-Fi / Remote control expansion board |
| **H-Bridge Motor Driver** | 1 per robot | L298N / TB6612FNG / MX1508 motor driver modules |
| **Ultrasonic Sensors** | 1–2 | HC-SR04 / US-015 distance sensors for object/enemy detection |
| **IR Reflectance Array** | 1 per robot | TCRT5000 / QTR sensor modules for edge and line detection |
| **Power Supply** | 1 per robot | 2S/3S LiPo battery or high-current 18650 power pack |

---

## ⚡ Compilation & Setup Guide

### 1. Prerequisites
- Install [VS Code](https://code.visualstudio.com/).
- Install the **PlatformIO IDE** extension.
- Install the **DashIO** mobile app on your Android or iOS device.

### 2. Flashing a Robot Module
1. Clone this repository:
   ```bash
   git clone https://github.com/TheAfricanJiant/Eviatech_2025.git
   cd Eviatech_2025
   ```
2. Open the desired robot directory (e.g., `Race Robot` or `Inginuity_Robot_boat`) in VS Code via PlatformIO.
3. Verify board configuration in `platformio.ini`:
   ```ini
   [env:nano33ble]
   platform = nordicnrf52
   board = nano33ble
   framework = arduino
   ```
4. Build and upload firmware to your board.

---

## 🔗 Connected Projects

- 🦾 [**Practical C++ — BraccioV2 Masterclass**](https://github.com/TheAfricanJiant/Practical-C-): Foundational C++ robotics firmware course.
- 🤖 [**Pick & Place — Edge Impulse AI**](https://github.com/TheAfricanJiant/Pick_and_Place): Edge Impulse computer vision object localization.
- 🚀 [**ROS 2 Ten Months Challenge**](https://github.com/TheAfricanJiant/ROS--2-Ten-Months-Challenge): Micro-ROS teleoperation and autonomous mobile robotics.
- 🐾 [**Project NYX**](https://github.com/TheAfricanJiant/Project_NYX_): Passive Wi-Fi CSI motion tracking & security watchdog.

---

<div align="center">

*Designed and developed by [Tambu Precious Takum](https://www.linkedin.com/in/tambu-precious-29bb67217/) for Eviatech 2025.*

</div>
