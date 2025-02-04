#ifndef IRSENSOR_H
#define IRSENSOR_H

#include "Sensor.h"
#include <Arduino.h>

class IRSensor : public Sensor{
    private:
        const uint8_t pin; 
    
    public: 
        IRSensor(const uint8_t inpin) : pin(inpin){
            pinMode(pin, INPUT); 
        }

        int read() override{
            return analogRead(pin);
        }
};

#endif