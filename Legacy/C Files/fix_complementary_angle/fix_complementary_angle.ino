#include "Arduino_BMI270_BMM150.h"
#include "sensor_reading_class.h" // contains SensorReading class definition
#include "calculate_tilt_angle_functions.h" // contains functions to calculate tilt angle
//#include "motor_control_functions.h" // contains functions to control motor
#include <PID_v1.h>

int counter = -1;
float acc_tilt_angle;
float gyro_change_angle;
float first_gyro_angle;
float gyro_tilt_angle;
float complementary_tilt_angle;
float k = 0.05;
float SampleRate;

// ---------------  Pin Assignments  ---------------
const int pwmA1 = 9;   // PWM output for Motor A
const int pwmA2 = 10;  // PWM output for Motor A
const int pwmB1 = 5;   // PWM output for Motor B
const int pwmB2 = 6;   // PWM output for Motor B

// ---------------  PID Tuning  ---------------
float Kp = 9; //10
float Ki = 0; //60; //140
float Kd = 0; //0.2; //0.22

// ---------------  Funtion Declaration  ---------------

void moveWheelsBackward(int pwmInput) {
  analogWrite(pwmA1, 255);
  analogWrite(pwmA2, 255-pwmInput);
  analogWrite(pwmB1, 255);
  analogWrite(pwmB2, 255-pwmInput);
}

void moveWheelsForward(int pwmInput) {
  analogWrite(pwmA1, 255-pwmInput);
  analogWrite(pwmA2, 255);
  analogWrite(pwmB1, 255-pwmInput);
  analogWrite(pwmB2, 255);
}

void stopWheels() {
  analogWrite(pwmA1, 255);
  analogWrite(pwmA2, 255);
  analogWrite(pwmB1, 255);
  analogWrite(pwmB2, 255);
}


// ---------------  PID Library instantiation  ---------------
double compAnglePID, PWM, Setpoint = 0.0;
PID myPID(&compAnglePID, &PWM, &Setpoint, Kp, Ki, Kd, DIRECT);

void setup() {
    pinMode(pwmA1, OUTPUT);
    pinMode(pwmA2, OUTPUT);
    pinMode(pwmB1, OUTPUT);
    pinMode(pwmB2, OUTPUT);
    
    Serial.begin(9600);
    
    // Initialize IMU before creating sensor objects
    if (!SensorReading::initIMU()) {
        while (1); // Halt if initialization fails
        Serial.print("Initialization failed!");
    }   

    // PID library related setups
    myPID.SetOutputLimits(-255, 255);
    myPID.SetSampleTime(10);
    myPID.SetMode(AUTOMATIC);
    SampleRate = IMU.gyroscopeSampleRate();
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
//  Serial.print(acc_tilt_angle); Serial.print('\t');

  gyro_change_angle = calc_gyro_change_angle(lastSensorInstance.timestamp, sensorInstance.timestamp, sensorInstance.xg);
//  Serial.print(gyro_change_angle); Serial.print('\t');

//  if (counter % 1000 == 0) {
//    gyro_tilt_angle = acc_tilt_angle;
//    Serial.print(gyro_tilt_angle); Serial.print('\t'); Serial.print("RESET"); Serial.print('\t');
//  }
//  else {
//     gyro_tilt_angle = calc_gyro_tilt_angle(gyro_tilt_angle, lastSensorInstance.timestamp, sensorInstance.timestamp, sensorInstance.xg);
//     Serial.print(gyro_tilt_angle); Serial.println('\t'); 
//  }

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

  compAnglePID = complementary_tilt_angle;
  //Serial.println(complementary_tilt_angle);
  myPID.Compute();

  if (complementary_tilt_angle > 0) {
    moveWheelsBackward(abs(PWM));
  } else if (complementary_tilt_angle < 0) {
    moveWheelsForward(abs(PWM));
  } else {
    stopWheels();
  }
}
