#include "Arduino_BMI270_BMM150.h"
#include "sensor_reading_class.h"
#include "calculate_tilt_angle_functions.h"

// declare global instances of type SensorReading 
SensorReading sensorInstance;
SensorReading lastSensorInstance;

// initialize global variables
int counter = -1;
float acc_tilt_angle;
float gyro_tilt_angle;
float last_gyro_tilt_angle;
float complementary_angle;
float last_complementary_angle;
float k = 0.8; // modify later

void setup() {

  // initialize IMU
  IMU.begin();
  if (!IMU.begin()) {
    Serial.println("Failed to initialize IMU!");
    while (1);
  }
}

void loop() {
    
  counter++;

  sensorInstance.readSensors();
  sensorInstance.displaySensorReadings();
  float acc_tilt_angle = calc_acc_tilt_angle(sensorInstance.ya, sensorInstance.za);
  Serial.println(acc_tilt_angle);

  if (counter % 20 == 0) {
    gyro_tilt_angle = acc_tilt_angle;
  }
  else {
    gyro_tilt_angle = calc_gyro_tilt_angle(last_gyro_tilt_angle, lastSensorInstance.timestamp, sensorInstance.timestamp, sensorInstance.zg);
    Serial.println(gyro_tilt_angle);
  }

  // after all calculations are done
  if(counter != 1) {
    lastSensorInstance = sensorInstance;
    lastSensorInstance.displaySensorReadings();
  }

  last_gyro_tilt_angle = gyro_tilt_angle;

  // remove comments once sensors have been read and angles have been mapped - then test comp angle
  // if(counter % 20 == 0) {
  //   complementary_angle = acc_tilt_angle;
  // }
  // else {
  //   newComplementaryAngle = k * (lCA + (gyro_tilt_angle)) + (1 - k) * acc_tilt_angle;
  // }

} 
