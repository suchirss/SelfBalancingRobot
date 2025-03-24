#include "Arduino_BMI270_BMM150.h"
#include "sensor_reading_class.h" // contains SensorReading class definition
#include "calculate_tilt_angle_functions.h" // contains functions to calculate tilt angle

int counter = -1;
float acc_tilt_angle;
float gyro_tilt_angle;
float last_gyro_tilt_angle;
bool ginitialized = false;
float complementary_tilt_angle;
float k = 0.7;

void setup() {
    delay(10000);
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

  // calculate gyroscope and complementary tilt angle
  if (counter % 20 == 0) {
    last_gyro_tilt_angle = acc_tilt_angle;
    complementary_tilt_angle = acc_tilt_angle;
    Serial.print(last_gyro_tilt_angle); Serial.print('\t'); Serial.print("RESET");  
  }
  else {
    gyro_tilt_angle = calc_gyro_tilt_angle(last_gyro_tilt_angle, lastSensorInstance.timestamp, sensorInstance.timestamp, sensorInstance.xg);
    Serial.println(gyro_tilt_angle); 
    last_gyro_tilt_angle = gyro_tilt_angle; // set last_gyro_tilt_angle
  }

  // class memory updating after all calculations are done
  lastSensorInstance = sensorInstance;
  ginitialized = true;

  
  // calculate complementary angle
  Serial.print(k); Serial.print('\t'); Serial.print(gyro_tilt_angle); Serial.print('\t'); 
  Serial.print(acc_tilt_angle); Serial.print('\t'); Serial.println(complementary_tilt_angle);
  complementary_tilt_angle = (k * (complementary_tilt_angle + gyro_tilt_angle)) + ((1 - k) * acc_tilt_angle);
//  complementary_tilt_angle = (k, gyro_tilt_angle, acc_tilt_angle, complementary_tilt_angle);
  Serial.print('\t'); Serial.println(complementary_tilt_angle);

}
