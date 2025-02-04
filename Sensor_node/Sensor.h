#ifndef SENSOR_H
#define SENSOR_H

#include <Arduino.h>

class Sensor{
    public:
        virtual int read() = 0; 
        virtual ~Sensor() {}
};

#endif