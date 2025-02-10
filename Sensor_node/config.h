/******************************************************************
* Project: Eviatech 2025 Competion                                *
* -----                                                           *
* Authors: Tambu Precious and Asogna Brilan 2025                  *                               *
* -----                                                           *
* Goal: Config file with pin inclusion                            *
*******************************************************************/

//#pragma once
#ifndef CONFIG_H
#define CONFIG_H

#include <Arduino.h>

// Hardware configuration for Arduino nano running the new bootloader.
// IO configuration

// Motor pin configuration
const uint8_t left_motorA = 5;
const uint8_t left_motorB = 4;
const uint8_t left_motorPWM = 9;
const uint8_t right_motorA = 6;
const uint8_t right_motorB = 7;
const uint8_t right_motorPWM = 8;

// Encoder pin configuration

#define MAX_SENSORS 10 //  count all number of sensors (ultrasonics + IRs)

// Infrared sensor pin configuration
// the sensor ADC channels in case we have no special use for a given channel
const uint8_t IR_0 = A0;
const uint8_t IR_1 = A1;
const uint8_t IR_2 = A2;
const uint8_t IR_3 = A3;
const uint8_t IR_4 = A4;
const uint8_t IR_5 = A5; // others
const uint8_t switches = A6;
const uint8_t battery = A7;

const int IR_threshold = 800; // seperation between black and white
// Ultrasonic pin configuration
const uint8_t frontA_trig = 2;
const uint8_t frontA_echo = 3;
const uint8_t frontB_trig = 10;
const uint8_t frontB_echo = 11;


// Control Button

// Program constants conversion
const float RADIANS_PER_DEGREE = 2 * PI / 360;
const float DEGREES_PER_RADIAN = 360 / 2 * PI;

//Program variables
const int maxSpeed = 255;
const int minSpeed = 110;
 #endif



