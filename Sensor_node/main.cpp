/******************************************************************
* Project: Eviatech 2025 Competion                                *
* -----                                                           *
* Authors: Tambu Precious and Asogna Brilan 2025                  *                               *
* -----                                                           *
* Goal: A fast Summo Wrestling Robot                              *
*******************************************************************/

#include <Arduino.h>
#include "IRSensor.h"
#include "ULTRAsonic.h"
#include "Sensors.h"
#include "config.h"
#include "motors.h"


Sensors sensors; 
int Ultrasonicsvalues[5]; // Adjust according to number of sensors
int Irsensorvalues[5]; // IR 

//IR sensors 
IRSensor A0_(IR_0); 
IRSensor A1_(IR_1); 
IRSensor A2_(IR_2); 
IRSensor A3_(IR_3); 
IRSensor A4_(IR_4); 

//utrasonic sensors
Ultrasonic frontSonar(frontA_trig, frontA_echo); 
Ultrasonic backSonar(frontB_trig, frontB_echo);

void setup(){
    Serial.begin(9600);

    // you can add sensors in any order, they will be classified and tracked for fast iteration during reads.
    
    sensors.addSensor(&A0_); // parsing by refrence , to avoid creating copies
    sensors.addSensor(&A1_);
    sensors.addSensor(&A2_);
    sensors.addSensor(&A3_);
    sensors.addSensor(&A4_);
    
    sensors.addSensor(&frontSonar);
    sensors.addSensor(&backSonar);
   
   // Initialize motors
    motors.initMotorPins();
 
}

void loop(){
    sensors.readAll(SensorType::IR, Irsensorvalues); // read all IR sensors and save it 
    sensors.readAll(SensorType::Ultrasonic,  Ultrasonicsvalues); // read all ultrasonics
}







/***************************** ALGORITHM *************************************************
*
*
*
*
*
*
*
*
*
