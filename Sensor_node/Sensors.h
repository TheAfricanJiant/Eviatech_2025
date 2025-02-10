#ifndef SENSORS_H
#define SENSORS_H

#include <Arduino.h>
#include "Sensor.h"
#include "config.h" 

//in future, make this code to able to work with any sensor as long as it is part of a base class , like Kwargs

class Sensors{
    private:
        Sensor* sensors[MAX_SENSORS];
        uint8_t sensorCount = 0;

        //indices to seperate sensors into categories on add, to reduce cost by not having to loop through all of the sensors[]
        uint8_t irStartIndex = 0; 
        uint8_t irEndIndex = 0; 
        uint8_t ultrasonicStartIndex = 0; 
        uint8_t ultrasonicEndIndex = 0; 

public:
    void addSensor(Sensor* sensor){
        if(sensorCount < MAX_SENSORS) { 
            sensors[sensorCount] = sensor; 
            if(sensor->getType() == SensorType::IR){ 
                if(irStartIndex == irEndIndex){
                    irStartIndex = sensorCount;
                }
                irEndIndex = sensorCount + 1;

            } else if(sensor->getType() == SensorType::Ultrasonic){
                if(ultrasonicStartIndex == ultrasonicEndIndex){
                    ultrasonicStartIndex = sensorCount;
                }
                ultrasonicEndIndex =  sensorCount + 1; 
            }
            sensorCount++; 
            
        }
    }

    void readAll(SensorType type, int* values){
        uint8_t startIndex , endIndex; 
        if(type == SensorType::IR){
            startIndex = irStartIndex; 
            endIndex = irEndIndex;

        } else if(type == SensorType::Ultrasonic){
            startIndex = ultrasonicStartIndex; 
            endIndex = ultrasonicEndIndex;
        } else {
            // add more sensors here in future
            return; 
        }

        for(uint8_t i = startIndex;  i < endIndex; i++) { values[i] = sensors[i]->read(); }
    }
};

#endif 