#include "Arduino_BMI270_BMM150.h"
#include "sensor_reading_class.h" // contains SensorReading class definition
#include "calculate_tilt_angle_functions.h" // contains functions to calculate tilt angle

int counter = -1;
float acc_tilt_angle;
float gyro_change_angle;
float complementary_tilt_angle;
float k = 0.7;

void setup() {
    Serial.begin(9600);
    
    // Initialize IMU before creating sensor objects
    if (!SensorReading::initIMU()) {
        while (1); // Halt if initialization fails
        Serial.print("Initialization failed!");
    }   
}

void loop() {
  // declare class variables
  SensorReading sensorInstance;
  static SensorReading lastSensorInstance; // static is used to ensure value is retained in next loop iteration

  counter++; // update counter

  // read sensors and display
  sensorInstance.readSensors();
//  sensorInstance.displaySensorReadings();

  // calculate accelerometer tilt angle
  acc_tilt_angle = calc_acc_tilt_angle(sensorInstance.ya, sensorInstance.za);
  Serial.print(acc_tilt_angle); Serial.print('\t');

  gyro_change_angle = calc_gyro_change_angle(lastSensorInstance.timestamp, sensorInstance.timestamp, sensorInstance.xg);
  Serial.print(gyro_change_angle); Serial.print('\t');

  // calculate gyroscope and complementary tilt angle
  if (counter == 0) {
    complementary_tilt_angle = acc_tilt_angle;
  }
  else {
    complementary_tilt_angle = calc_comp_angle(k, gyro_change_angle, acc_tilt_angle, complementary_tilt_angle);
    Serial.print(complementary_tilt_angle); Serial.println('\t');
  }

  // class memory updating after all calculations are done
  lastSensorInstance = sensorInstance;
}
