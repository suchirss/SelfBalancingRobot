#include "Arduino_BMI270_BMM150.h"
#include "sensor_reading_class.h"
#include "calculate_tilt_angle_functions.h"

// declare a global SensorReading instance
SensorReading sensorInstance;

float acc_tilt_angle;
float gyro_tilt_angle;
float last_gyro_tilt_angle;
  
void setup() {
  IMU.begin();
  if (!IMU.begin()) {
    Serial.println("Failed to initialize IMU!");
    while (1);
}
}

void loop() {

  sensorInstance.readSensors();
  sensorInstance.displaySensorReadings();
  float acc_tilt_angle = calc_acc_tilt_angle(sensorInstance.ya, sensorInstance.za);
  Serial.println(acc_tilt_angle);

} 
