#ifndef SENSOR_READING_H
#define SENSOR_READING_H

#include <Arduino.h>

class SensorReading {
    public:
        float xa, ya, za; // accelerometer readings in 3 dimensions
        float xg, yg, zg; // gyroscope readings in 3 dimensions
        unsigned long timestamp; // time of class instance (time of sensor reading)

    // Constructor declaration
    SensorReading();

    // declaration of readSensors function
    void readSensors();

    // declaration of print function
    void displaySensorReadings();

};

#endif