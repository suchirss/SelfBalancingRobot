#include "Arduino_BMI270_BMM150.h"
#include "sensor_reading_class.h"

// Constructor implementation
SensorReading::SensorReading() {
    readSensors();
}

void SensorReading::readSensors() {
    if (IMU.readAcceleration(xa, ya, za) && IMU.readGyroscope(xg, yg, zg)) {
        timestamp = millis();
    }
    else {
        Serial.println("Sensor readings failed!");
    }
}

void SensorReading::displaySensorReadings() {
    Serial.print(xa); Serial.print('\t');
    Serial.print(ya); Serial.print('\t');
    Serial.print(za); Serial.print('\t');
    Serial.print(xg); Serial.print('\t');
    Serial.print(yg); Serial.print('\t');
    Serial.print(zg); Serial.print('\t');
    Serial.println(timestamp);
}
