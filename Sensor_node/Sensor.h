#ifndef SENSOR_H
#define SENSOR_H

#include <Arduino.h>

enum class SensorType{IR, Ultrasonic};

class Sensor{
    private:
        SensorType type; 

    public:
        Sensor(SensorType type) : type(type){}
        virtual int read() = 0; 
        virtual ~Sensor() {}
        SensorType getType() const {return type; }
};
#endif