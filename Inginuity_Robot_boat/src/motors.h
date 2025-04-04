/******************************************************************
* Project: Eviatech 2025 Competion/Summo                          *
* -----                                                           *
* Authors: Tambu Precious 2025                                    *                               
* -----                                                           *
* Goal: A fast Summo Wrestling Robot                              *
*******************************************************************/

// Header files
#ifndef MOTORS_H
#define MOTORS_H

#define FIN_LEFT  0
#define FIN_RIGHT  120
#define FIN_STRAIGHT 50


#include <Arduino.h>
//#include "battery.h"
#include "config.h"
#include <Servo.h>

Servo ESC; // eNGINE 
Servo Fin; 
Servo Secure; 

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

        ESC.attach(Weapon_pin); // engine
        Fin.attach(Fin_pin);

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
    void BLDCMotorControl(int angularSpeed){
        ESC.write(angularSpeed);
    }
    void secureBalls(int angle){
        Secure.write(angle);
    }
    void Left_Right(int dirrection){
        if(dirrection==1){ // RIGHT
        Fin.write(FIN_LEFT); 
          }
        else if (dirrection == 0){
            Fin.write(FIN_RIGHT); // left 
        }
        else{
            Fin.write(FIN_STRAIGHT); // center fin , move straight 
        }
    }


    };
#endif
