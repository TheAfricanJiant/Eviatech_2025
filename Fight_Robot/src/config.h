/******************************************************************
 * Project: ElviaTech National Robotics Competition Code Suit      *
 * -----                                                           *
 * Author: Tambu Precious, 2025                                *
 * -----                                                           *
 * Goal: Learn C++ & Robotics, and be the winner                   *
 *******************************************************************/

// #pragma once
#ifndef CONFIG_H
#define CONFIG_H

#include <Arduino.h>

/***********************************************************
 * Hardware configuration for Arduino 33 BLE.
 * IO configuration
 ************************************************************/

//================== Motor pin configuration =============================//
const uint8_t rightMotorA = 10;
const uint8_t rightMotorB = 9;
const uint8_t rightMotorPWM = 6;
const uint8_t leftMotorA = 8;
const uint8_t leftMotorB = 7;
const uint8_t leftMotorPWM = 5;

const uint8_t Weapon_pin = 11; 

//================== Motor pin configuration end=========================//

//======================== Infrared sensor pin configuration ================================//
const uint8_t IR_0 = A0; // left IR sensor
const uint8_t IR_1 = A1;
const uint8_t IR_2 = A2;
const uint8_t IR_3 = A3;
const uint8_t IR_4 = A4; // right IR sensor

//=================================== END of IR Sensor =====================================//

//========== Ultrasonic sensor pin configuration & variable decalarations ==================//
const uint8_t R_echo = A0;
const uint8_t R_trig = A1;
const uint8_t F_echo = A2;
const uint8_t F_trig = A3;
const uint8_t L_echo = A4;
const uint8_t L_trig = A5;

int frontDist{0}, rightDist{0}, leftDist{0};
extern void R_echoISR();
extern void F_echoISR();
extern void L_echoISR();
extern void triggerSensor(int);

/* Distance measurement variables*/
volatile long FDuration{0}, RDuration{0}, LDuration{0};
volatile bool FReceived = false, RReceived = false, LReceived = false;
unsigned long lastTriggerTime{0};
int sensorIndex = 0; // 0 = left, 1 = front, 2 = right

/* Ultrasonic sensor operating limits */
int ut_min{2};
int ut_max{400};
//---------------------- END of Ultrasonic Sensor --------------------

const uint8_t switches = A6;
const uint8_t battery_pin = A7;

// --------------------- Control Button ------------------------------

// --------------- Program constants conversion ----------------------
const float RADIANS_PER_DEGREE = 2 * PI / 360;
const float DEGREES_PER_RADIAN = 360 / 2 * PI;

// Program variables
const int maxSpeed = 255;
const int minSpeed = 110;

// Onboard LED pin
const int onboardLED = 13;

// UI buttons
const int button1 = 11;
const int button2 = 12;

// Potentiometer
int potValue;

#endif
