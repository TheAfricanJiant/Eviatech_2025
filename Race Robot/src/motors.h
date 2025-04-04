/******************************************************************
* Project: Eviatech 2025 Competion/Summo                          *
* -----                                                           *
* Authors: Asogna Brilan and Tambu Precious 2025                                     *                               
* -----                                                           *
* Goal: A fast Summo Wrestling Robot                              *
*******************************************************************/

// Header files
#ifndef MOTORS_H
#define MOTORS_H

#include <Arduino.h>
//#include "battery.h"
#include "config.h"

class Motors;
extern Motors motors;

class Motors{
    public:

    void initMotorPins(){
        pinMode(leftMotorA, OUTPUT);
        pinMode(leftMotorB, OUTPUT);
        pinMode(rightMotorB, OUTPUT);
        pinMode(rightMotorA, OUTPUT);
        
        pinMode(leftMotorPWM, OUTPUT);
        pinMode(rightMotorPWM, OUTPUT);

        digitalWrite(leftMotorPWM, 0);
        digitalWrite(rightMotorPWM, 0);
    }

    // simple movement commands
    // if Direction == 0, motor spins backward
    // if Direction == 1, motor spins forward
    void rightMotor(int PWM, int Direction){
        if (Direction == 1){
            analogWrite(rightMotorPWM, PWM);
            digitalWrite(rightMotorA, 1);
            digitalWrite(rightMotorB, 0);
        }

        if (Direction == 0){
            analogWrite(rightMotorPWM, PWM);
            digitalWrite(rightMotorA, 0);
            digitalWrite(rightMotorB, 1);
        }
    }

    void leftMotor(int PWM, int Direction){
        if (Direction == 1){
            analogWrite(leftMotorPWM, PWM);
            digitalWrite(leftMotorA, 0);
            digitalWrite(leftMotorB, 1);
        }

        if (Direction == 0){
            analogWrite(leftMotorPWM, PWM);
            digitalWrite(leftMotorA, 1);
            digitalWrite(leftMotorB, 0);
        }
    }

    void stop(){
        analogWrite(rightMotorPWM, 0); 
        analogWrite(leftMotorPWM, 0); 
    }
    
};
#endif
