#include "Arduino_BMI270_BMM150.h"
#include "sensor_reading_class.h"
#include "calculate_tilt_angle_functions.h"
#include <PID_v1.h>
#include <ArduinoBLE.h>

// -- BLUETOOTH -- //

#define BUFFER_SIZE 20

// Define a custom BLE service and characteristic
BLEService customService("00000000-5EC4-4083-81CD-A10B8D5CF6EC");
BLECharacteristic customCharacteristic(
    "00000001-5EC4-4083-81CD-A10B8D5CF6EC", BLERead | BLEWrite | BLENotify, BUFFER_SIZE, false);

bool isCentralConnected = False;

// -- BLUETOOTH END -- //

// ---------------  Pin Assignments  ---------------
const int pwmA1 = 9;   // PWM output for Motor A
const int pwmA2 = 10;  // PWM output for Motor A
const int pwmB1 = 5;   // PWM output for Motor B
const int pwmB2 = 6;   // PWM output for Motor B


// ---------------  PID Tuning  ---------------
float Kp = 15; //10
float Ki = 400; //140
float Kd = 0.01; //0.22

// ---------------  Variable Declarations  ---------------
int counter = -1;
float k = 0.9;
  
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


// ---------------------------------------------------------------------------------------------------------------------------------------


void setup() {
    pinMode(pwmA1, OUTPUT);
    pinMode(pwmA2, OUTPUT);
    pinMode(pwmB1, OUTPUT);
    pinMode(pwmB2, OUTPUT);


    // -- BLUETOOTH -- //
    Serial.begin(9600);
    while (!Serial);

    // Initialize the built-in LED to indicate connection status
    pinMode(LED_BUILTIN, OUTPUT);

    if (!BLE.begin()) {
      Serial.println("Starting BLE failed!");
      while (1);
    }
  
    // Set the device name and local name
    BLE.setLocalName("BLE-BERT-AND-ERNIE");
    BLE.setDeviceName("BLE-BERT-AND-ERNIE");
  
    // Add the characteristic to the service
    customService.addCharacteristic(customCharacteristic);
  
    // Add the service
    BLE.addService(customService);
  
    // Set an initial value for the characteristic
    customCharacteristic.writeValue("Waiting for data");
  
    // Start advertising the service
    BLE.advertise();
  
    Serial.println("Bluetooth® device active, waiting for connections...");

    // -- BLUETOOTH END -- //   

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

void loop() {

  // -- BLUETOOTH -- //

  // Wait for a BLE central to connect
  BLEDevice central = BLE.central();

  if (central) {
    if (isCentralConnected == False) {
      isCentralConnected == True;
      Serial.print("Connected to central: ");
      Serial.println(central.address());
      digitalWrite(LED_BUILTIN, HIGH); // Turn on LED to indicate connection
    }
    
    // Run this in the loop if connected to bluetooth device
    if (central.connected()) {
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

        // Print the received data to the Serial Monitor
        Serial.print("Received data: ");
        Serial.println(receivedString);

        // Optionally, respond by updating the characteristic's value
        customCharacteristic.writeValue("Data received");
      }
    }
    else {
      isCentralConnected == False;
      digitalWrite(LED_BUILTIN, LOW); // Turn off LED when disconnected
      Serial.println("Disconnected from central.");
    }
  }
  
  // -- BLUETOOTH END -- //

   // Check if new PID values are available from Serial
  if (Serial.available()) {
    String input = Serial.readStringUntil('\n');  // Read user input
    parsePIDInput(input);  // Update PID values
  }
  
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
    // Serial.println(gyro_tilt_angle);
    //last_gyro_tilt_angle = gyro_tilt_angle; // set last_gyro_tilt_angle
  // }
  
  //IMU.readGyroscope(gyroX, gyroY, gyroZ);
  //gyro_tilt_angle = (1.0/SampleRate)*gyroX + comp_angle;
  // class memory updating after all calculations are done
  lastSensorInstance = sensorInstance;
  ginitialized = true;
  
  comp_angle = (k * (gyro_tilt_angle)) + ((1.0 - k) * acc_tilt_angle);
  // Serial.print(comp_angle); Serial.print('\t');

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

  if (comp_angle > 0) {
    moveWheelsBackward(abs(PWM));
  } else if (comp_angle < 0) {
    moveWheelsForward(abs(PWM));
  } else {
    stopWheels();
  }
}

// ---------------------------------------------------------------------------------------------------------------------------------------
// Function to parse Serial input and update PID values
void parsePIDInput(String input) {
  float newKp, newKi, newKd;
  int kpIndex = input.indexOf("Kp=");
  int kiIndex = input.indexOf("Ki=");
  int kdIndex = input.indexOf("Kd=");

  if (kpIndex != -1 && kiIndex != -1 && kdIndex != -1) {
    newKp = input.substring(kpIndex + 3, input.indexOf(" ", kpIndex)).toFloat();
    newKi = input.substring(kiIndex + 3, input.indexOf(" ", kiIndex)).toFloat();
    newKd = input.substring(kdIndex + 3).toFloat();

    // Update PID values
    Kp = newKp;
    Ki = newKi;

    // Handle cases where there may not be a space after the Kd value
    if (kdIndex != -1) {
      newKd = input.substring(kdIndex + 3).toFloat();
    } else {
      newKd = 0.0; // Default value if Kd is not provided
    }

    myPID.SetTunings(Kp, Ki, Kd);

    Serial.print("Updated PID values -> ");
    Serial.print("Kp: "); Serial.print(Kp);
    Serial.print(" Ki: "); Serial.print(Ki);
    Serial.print(" Kd: "); Serial.println(Kd);
  } else {
    Serial.println("Invalid input format! Use: Kp=10 Ki=500 Kd=0.05");
  }
}

//void setupBluetooth() {
//  Serial.begin(9600);
//  while (!Serial);
//
//  // Initialize the built-in LED to indicate connection status
//  pinMode(LED_BUILTIN, OUTPUT);
//
//  if (!BLE.begin()) {
//    Serial.println("Starting BLE failed!");
//    while (1);
//  }
//
//  // Set the device name and local name
//  BLE.setLocalName("BLE-BERT-AND-ERNIE");
//  BLE.setDeviceName("BLE-BERT-AND-ERNIE");
//
//  // Add the characteristic to the service
//  customService.addCharacteristic(customCharacteristic);
//
//  // Add the service
//  BLE.addService(customService);
//
//  // Set an initial value for the characteristic
//  customCharacteristic.writeValue("Waiting for data");
//
//  // Start advertising the service
//  BLE.advertise();
//
//  Serial.println("Bluetooth® device active, waiting for connections...");
//}
