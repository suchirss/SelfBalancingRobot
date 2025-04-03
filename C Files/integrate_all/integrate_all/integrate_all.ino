#include "Arduino_BMI270_BMM150.h"
#include "sensor_reading_class.h"
#include "calculate_tilt_angle_functions.h"
#include <PID_v1.h>
#include <ArduinoBLE.h>

#define BUFFER_SIZE 20

// // Define a custom BLE service and characteristic
BLEService customService("00000000-5EC4-4083-81CD-A10B8D5CF6EC");
BLECharacteristic customCharacteristic(
    "00000001-5EC4-4083-81CD-A10B8D5CF6EC", BLERead | BLEWrite | BLENotify, BUFFER_SIZE, false);
String receivedCommand = "";
unsigned long lastStopTime = 0;

// ---------------  Pin Assignments  ---------------
const int pwmA1 = 9;   // PWM output for Motor A
const int pwmA2 = 10;  // PWM output for Motor A
const int pwmB1 = 5;   // PWM output for Motor B
const int pwmB2 = 6;   // PWM output for Motor B

float leftPWM = 0, rightPWM = 0, forwardPWM = 0, backwardPWM = 0;
int forwardBias = 0;

// ---------------  PID Tuning  ---------------
float Kp = 5;    //15-25
float Ki = 200; //250;   //400
float Kd = 0.05; //0.01;  //0.01

// ---------------  Variable Declarations  ---------------
float k = 0.95;

float acc_tilt_angle, gyro_tilt_angle, last_gyro_tilt_angle, comp_angle, SampleRate, gyroX, gyroY, gyroZ;

// ---------------  PID Library instantiation  ---------------
double compAnglePID, PWM, Setpoint = 0.0;
PID myPID(&compAnglePID, &PWM, &Setpoint, Kp, Ki, Kd, DIRECT);
unsigned long startTime = 0;
bool flag;

void setup() {
  pinMode(pwmA1, OUTPUT);
  pinMode(pwmA2, OUTPUT);
  pinMode(pwmB1, OUTPUT);
  pinMode(pwmB2, OUTPUT);

  Serial.begin(9600);
  // Initialize IMU before creating sensor objects
  if (!SensorReading::initIMU()) {
    while (1)
      ;  // Halt if initialization fails
  }

  // PID library related setups
  myPID.SetOutputLimits(-255, 255);
  myPID.SetSampleTime(10);
  myPID.SetMode(AUTOMATIC);
  SampleRate = IMU.gyroscopeSampleRate();

  BLE.begin();

  // Set the device name and local name
  BLE.setLocalName("BLE-BERT-AND-ERNIE");
  BLE.setDeviceName("BLE-BERT-AND-ERNIE");

  // Add the characteristic to the service
  customService.addCharacteristic(customCharacteristic);

  // Add the service
  BLE.addService(customService);

  // Start advertising the service
  BLE.advertise();
}

void loop() {
  bleControl();

  // declare class variables
  SensorReading sensorInstance;
  static SensorReading lastSensorInstance;  // static is used to ensure value is retained in next loop iteration

  // read sensors and display
  sensorInstance.readSensors();

  float acc_tilt_angle = calc_acc_tilt_angle(sensorInstance.ya, sensorInstance.za);

  gyro_tilt_angle = calc_gyro_tilt_angle(comp_angle, lastSensorInstance.timestamp, sensorInstance.timestamp, sensorInstance.xg);
  lastSensorInstance = sensorInstance;

  comp_angle = (k * (gyro_tilt_angle)) + ((1.0 - k) * acc_tilt_angle);

  compAnglePID = comp_angle;
  myPID.Compute();

  if (receivedCommand == "FORWARD") {
  Setpoint = 0.4;
  balance();
  }
  else if (receivedCommand == "BACKWARD") {
    Setpoint = -0.4;
    balance();
  }
  else if (receivedCommand == "LEFT") {
    // turn45deg();
    Setpoint = 0;

    if (comp_angle > 0) moveWheelsRight(abs(PWM));
    else if (comp_angle < 0) moveWheelsLeft(abs(PWM));
    else stopWheels();
  }
  else if (receivedCommand == "RIGHT") {
    // turn45deg();
    Setpoint = 0;

    if (comp_angle > 0) moveWheelsRight2(abs(PWM));
    else if (comp_angle < 0) moveWheelsLeft2(abs(PWM));
    else stopWheels();
  }
  else {
    Setpoint = 0;
    balance();
  }
}

// ---------------  Funtion Declaration  ---------------
void bleControl() {
  BLEDevice central = BLE.central();

  // Check if the characteristic was written
  if (customCharacteristic.written()) {
    // Get the length of the received data
    int length = customCharacteristic.valueLength();

    // Read the received data
    const unsigned char* receivedData = customCharacteristic.value();

    // Create a properly terminated string
    char receivedString[length + 1]; // +1 for null terminator
    memcpy(receivedString, receivedData, length);
    receivedString[length] = '\0'; // Null-terminate the string

    // receivedCommand += receivedString;
    receivedCommand = String(receivedString);
  }
}

// void turn45deg() {
//   if (flag) {
//     startTime = millis();
//     flag = 0;
//   }
//   if (!flag && millis() - startTime >= 500) {
//     receivedCommand = "";
//     flag = 1;
//   }
// }

void balance() {
  if (comp_angle > 0) moveWheelsBackward(abs(PWM));
  else if (comp_angle < 0) moveWheelsForward(abs(PWM));
  else stopWheels();
}

void moveWheelsBackward(int pwmInput) {
  analogWrite(pwmA1, 255);
  analogWrite(pwmA2, 255 - pwmInput);
  analogWrite(pwmB1, 255);
  analogWrite(pwmB2, 255 - pwmInput);
}

void moveWheelsForward(int pwmInput) {
  analogWrite(pwmA1, 255 - pwmInput);
  analogWrite(pwmA2, 255);
  analogWrite(pwmB1, 255 - pwmInput);
  analogWrite(pwmB2, 255);
}

// void moveWheelsBackward(float leftPWM, float rightPWM) {
//   analogWrite(pwmA1, 255);
//   analogWrite(pwmA2, 255 - rightPWM);
//   analogWrite(pwmB1, 255);
//   analogWrite(pwmB2, 255 - leftPWM);
// }

// void moveWheelsForward(float leftPWM, float rightPWM) {
//   analogWrite(pwmA1, 255 - rightPWM);
//   analogWrite(pwmA2, 255);
//   analogWrite(pwmB1, 255 - leftPWM);
//   analogWrite(pwmB2, 255);
// }

void stopWheels() {
  analogWrite(pwmA1, 255);
  analogWrite(pwmA2, 255);
  analogWrite(pwmB1, 255);
  analogWrite(pwmB2, 255);
}

void moveWheelsLeft(int pwmInput) {
  float newSpeed = 255 - pwmInput + 75;
  if (newSpeed > 255) newSpeed = 255;
  analogWrite(pwmA1, newSpeed - 75);
  analogWrite(pwmA2, 255);
  analogWrite(pwmB1, newSpeed);
  analogWrite(pwmB2, 255);
}

void moveWheelsRight(int pwmInput) {
  float newSpeed = 255 - pwmInput + 75;
  if (newSpeed > 255) newSpeed = 255;
  analogWrite(pwmA1, 255);
  analogWrite(pwmA2, newSpeed);
  analogWrite(pwmB1, 255);
  analogWrite(pwmB2, newSpeed-75);
}

void moveWheelsLeft2(int pwmInput) {
  float newSpeed = 255 - pwmInput + 75;
  if (newSpeed > 255) newSpeed = 255;
  analogWrite(pwmA1, newSpeed);
  analogWrite(pwmA2, 255);
  analogWrite(pwmB1, newSpeed - 75);
  analogWrite(pwmB2, 255);
}

void moveWheelsRight2(int pwmInput) {
  float newSpeed = 255 - pwmInput + 75;
  if (newSpeed > 255) newSpeed = 255;
  analogWrite(pwmA1, 255);
  analogWrite(pwmA2, newSpeed - 75);
  analogWrite(pwmB1, 255);
  analogWrite(pwmB2, newSpeed);
}
