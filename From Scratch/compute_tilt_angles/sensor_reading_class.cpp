#include "sensor_reading_class.h"

// Constructor: Initialize values to zero
SensorReading::SensorReading() : xa(0), ya(0), za(0), xg(0), yg(0), zg(0), timestamp(0) {}

// IMU Initialization Function (Call this once in setup)
bool SensorReading::initIMU() {
    if (!IMU_B.begin()) {
        Serial.println("Failed to initialize IMU!");
        return false;
    }
    return true;
}

// Reads accelerometer and gyroscope data
void SensorReading::readSensors() {
    if (IMU_B.readAcceleration(xa, ya, za) && IMU_B.readGyroscope(xg, yg, zg)) {
        timestamp = millis();
    } else {
        Serial.println("Sensor readings failed!");
    }
}

// Prints sensor values
void SensorReading::displaySensorReadings() {
    Serial.print(xa); Serial.print('\t');
    Serial.print(ya); Serial.print('\t');
    Serial.print(za); Serial.print('\t');
    Serial.print(xg); Serial.print('\t');
    Serial.print(yg); Serial.print('\t');
    Serial.print(zg); Serial.print('\t');
    Serial.println(timestamp);
}
