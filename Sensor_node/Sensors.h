#ifndef SENSORS_H
#define SENSORS_H

#include <Arduino.h>
#include "Sensor.h"
#include "config.h" 

class Sensors{
    private:
        Sensor* sensors[MAX_SENSORS];
        uint8_t sensorCount = 0; 

public:
    void addSensor(Sensor* sensor){
        if(sensorCount < MAX_SENSORS) { sensors[sensorCount++] = sensor; }
    }

    void readAll(int* values, uint8_t count){
        for(uint8_t i = 0;  i < count; i++) { values[i] = sensors[i]->read(); }
    }
};

#endif 