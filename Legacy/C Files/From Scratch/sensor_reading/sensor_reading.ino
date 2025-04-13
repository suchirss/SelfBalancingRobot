#include "Arduino_BMI270_BMM150.h"

float xa, ya, za; // accelerometer readings in 3 dimensions
float xg, yg, zg; // gyroscope readings in 3 dimensions
unsigned long timestamp; // time of class instance (time of sensor reading)

void setup() {
  // put your setup code here, to run once:
  // initialize IMU
  if (!IMU.begin()) {
    Serial.println("Failed to initialize IMU!");
    while (1);
  }
  
}

void readSensors() {
    if (IMU.readAcceleration(xa, ya, za) && IMU.readGyroscope(xg, yg, zg)) {
        timestamp = millis();
    }
    else {
        Serial.println("Sensor readings failed!");
    }
}

void displaySensorReadings() {
    Serial.print(xa); Serial.print('\t');
    Serial.print(ya); Serial.print('\t');
    Serial.print(za); Serial.print('\t');
    Serial.print(xg); Serial.print('\t');
    Serial.print(yg); Serial.print('\t');
    Serial.print(zg); Serial.print('\t');
    Serial.println(timestamp);
}

void loop() {
  readSensors();
  displaySensorReadings();

}
