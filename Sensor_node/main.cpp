/******************************************************************
* Project: Jonh-Deo     *
* -----                                                           *
* Author: Tambu Precious, 2025                                *
* -----                                                           *
* Goal: John-Deo                                                  *
*******************************************************************/

#include <Arduino.h>
#include "IRSensor.h"
#include "ULTRAsonic.h"
#include "Sensors.h"
#include "config.h"

Sensors sensors; 
int values[5]; // Adjust according to number of sensors 

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
    sensors.addSensor(&A0_); // parsing by refrence , to avoid creating copies
    sensors.addSensor(&A1_);
    sensors.addSensor(&A2_);
    sensors.addSensor(&A3_);
    sensors.addSensor(&A4_);
}

void loop(){
    sensors.readAll(values, 5);
}