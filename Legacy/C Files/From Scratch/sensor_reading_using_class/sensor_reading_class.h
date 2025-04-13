#ifndef SENSOR_READING_H
#define SENSOR_READING_H

#include <Arduino.h>
#include "Arduino_BMI270_BMM150.h"

class SensorReading {
    public:
        float xa, ya, za; // Accelerometer readings
        float xg, yg, zg; // Gyroscope readings
        unsigned long timestamp; // Timestamp of sensor reading

        // Constrsuctor
        SensorReading();

        // Initializes the IMU sensor
        static bool initIMU();

        // Reads data from sensors
        void readSensors();

        // Prints sensor readings
        void displaySensorReadings();
};

#endif