#include "Arduino_BMI270_BMM150.h"
#include "sensor_reading_class.h"

float xa, ya, za; // accelerometer readings in 3 dimensions
float xg, yg, zg; // gyroscope readings in 3 dimensions
unsigned long timestamp; // time of class instance (time of sensor reading)

void setup() {
    Serial.begin(9600);

    // Initialize IMU before creating sensor objects
    if (!SensorReading::initIMU()) {
        while (1); // Halt if initialization fails
    } 
}

void loop() {
  SensorReading sensorInstance;
  sensorInstance.readSensors();
  sensorInstance.displaySensorReadings();

}
