#ifndef ULTRASONIC_H
#define ULTRASONIC_H

#include <Arduino.h>
#include "Sensor.h"

class Ultrasonic : public Sensor{
    private:
        const uint8_t trigPin; 
        const uint8_t echoPin;

    public:
        Ultrasonic(uint8_t trig_, uint8_t echo_) :Sensor(SensorType::Ultrasonic),  trigPin(trig_), echoPin(echo_){
            pinMode(trigPin, OUTPUT); 
            pinMode(echoPin, INPUT);
        }

        int read() override{
            digitalWrite(trigPin, LOW); 
            delayMicroseconds(2); 
            digitalWrite(trigPin, HIGH); 
            delayMicroseconds(10);
            digitalWrite(trigPin, LOW);
            return pulseIn(echoPin, HIGH) / 58; //distance in cm
        }
};

#endif