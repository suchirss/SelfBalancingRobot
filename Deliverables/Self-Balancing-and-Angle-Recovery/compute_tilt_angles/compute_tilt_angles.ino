#include "Arduino_BMI270_BMM150.h"
#include "sensor_reading_class.h"
#include "calculate_tilt_angle_functions.h"
#include <PID_v1.h>

// ---------------  Pin Assignments  ---------------
const int pwmA1 = 9;   // PWM output for Motor A
const int pwmA2 = 10;  // PWM output for Motor A
const int pwmB1 = 5;   // PWM output for Motor B
const int pwmB2 = 6;   // PWM output for Motor B


// ---------------  PID Tuning  ---------------
float Kp = 15; //15-25
float Ki = 600; //400
float Kd = 0.01; //0.01

// ---------------  Variable Declarations  ---------------
int counter = -1;
float k = 0.90;

// good copy
// 2.75 2 0.2

float acc_tilt_angle, gyro_tilt_angle, last_gyro_tilt_angle, comp_angle, SampleRate, gyroX, gyroY, gyroZ;
bool ginitialized = false;


// ---------------  PID Library instantiation  ---------------
double compAnglePID, PWM, Setpoint = 0.0;
PID myPID(&compAnglePID, &PWM, &Setpoint, Kp, Ki, Kd, DIRECT);

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

void moveWheelsLeft(int pwmInput) {
  float newSpeed = 255 - pwmInput + 50;
  if (newSpeed > 255) newSpeed = 255;
  analogWrite(pwmA1, 255-pwmInput);
  analogWrite(pwmA2, 255);
  analogWrite(pwmB1, newSpeed);
  analogWrite(pwmB2, 255);
}

void moveWheelsRight(int pwmInput) {
  analogWrite(pwmA1, 255);
  analogWrite(pwmA2, 255);
  analogWrite(pwmB1, 255-pwmInput);
  analogWrite(pwmB2, 255);
}


// ---------------------------------------------------------------------------------------------------------------------------------------


void setup() {
    pinMode(pwmA1, OUTPUT);
    pinMode(pwmA2, OUTPUT);
    pinMode(pwmB1, OUTPUT);
    pinMode(pwmB2, OUTPUT);

    Serial.begin(9600);
    // Initialize IMU before creating sensor objects
    if (!SensorReading::initIMU()) {
        while (1); // Halt if initialization fails
    }   

    // PID library related setups
    myPID.SetOutputLimits(-255, 255);
    myPID.SetSampleTime(10);
    myPID.SetMode(AUTOMATIC);
    SampleRate = IMU.gyroscopeSampleRate();
}

unsigned long lastLoopTime = 0;  // Stores the last recorded time
unsigned long loopDuration = 0;  // Stores the time taken for one loop

void loop() {
  unsigned long currentTime = micros();  // Get current time in microseconds
  loopDuration = currentTime - lastLoopTime;  // Calculate loop execution time
  lastLoopTime = currentTime;  // Update last recorded time

  Serial.print("Loop time (us): ");
  Serial.println(loopDuration);  // Print loop time in microseconds

  // declare class variables
  SensorReading sensorInstance;
  static SensorReading lastSensorInstance; // static is used to ensure value is retained in next loop iteration

  //counter++; // update counter

  // read sensors and display
  sensorInstance.readSensors();
//  sensorInstance.displaySensorReadings();

  // calculate accelerometer tilt angle
  float acc_tilt_angle = calc_acc_tilt_angle(sensorInstance.ya, sensorInstance.za);
  // Serial.print(acc_tilt_angle); Serial.print('\t');

  // if (counter % 20 == 0) {
  //   last_gyro_tilt_angle = acc_tilt_angle;
    // Serial.print(last_gyro_tilt_angle); Serial.print('\t'); Serial.print("RESET"); Serial.print('\t');
  // }
  // else {
    gyro_tilt_angle = calc_gyro_tilt_angle(comp_angle, lastSensorInstance.timestamp, sensorInstance.timestamp, sensorInstance.xg);
    //Serial.println(gyro_tilt_angle);
    //last_gyro_tilt_angle = gyro_tilt_angle; // set last_gyro_tilt_angle
  // }
  
  //IMU.readGyroscope(gyroX, gyroY, gyroZ);
  //gyro_tilt_angle = (1.0/SampleRate)*gyroX + comp_angle;
  // class memory updating after all calculations are done
  lastSensorInstance = sensorInstance;
  ginitialized = true;
  
  // if(sqrt(sensorInstance.ya*sensorInstance.ya + sensorInstance.za*sensorInstance.za) > 1) {
  //   k = 1.0;
  // }
  // else {
  //   k = 0.9;
  // }
  
  comp_angle = (k * (gyro_tilt_angle)) + ((1.0 - k) * acc_tilt_angle);
  //Serial.print(comp_angle); Serial.print('\t');

  compAnglePID = comp_angle;
  Serial.println(comp_angle);
  myPID.Compute();

  // if (abs(PWM) > 10 && abs(PWM) < 40) {
  //   if (PWM > 0) PWM = 40;
  //   else PWM = -40;
  // }
  // else if (PWM < 10 && PWM > -10) PWM = 0;
  // if (comp_angle > -1 && comp_angle < 1) PWM = 0;
  // else if (comp_angle > 36 || comp_angle < -36) PWM = 0; // stop the robot if tilted too much
  // Serial.println(PWM);

  if (comp_angle > 0) moveWheelsBackward(abs(PWM));
  else if (comp_angle < 0) {
    moveWheelsForward(abs(PWM));
  }
  else stopWheels();

  
}