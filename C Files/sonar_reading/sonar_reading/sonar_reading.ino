#include "Arduino_BMI270_BMM150.h"
#include "sonar_reading_class.h"
#include <ArduinoBLE.h>

// initialize constants
#define BUFFER_SIZE 20

// initialize variables
bool isCentralConnected = false;
bool centralFlag = false;
int counter = -1;

// initialize six sonars
SonarReading sonarA(A2, A3); // correct
SonarReading sonarB(A6, A7); // correct
SonarReading sonarC(A0, A1); // correct
SonarReading sonarD(8, 7); // DOES NOT READ
SonarReading sonarE(A4, A5); // correct
SonarReading sonarF(D11, D12); // correct



// Define a custom BLE service and characteristic
BLEService customService("00000000-5EC4-4083-81CD-A10B8D5CF6EC");
BLECharacteristic customCharacteristic(
    "00000001-5EC4-4083-81CD-A10B8D5CF6EC", BLERead | BLEWrite | BLENotify, BUFFER_SIZE, false);

void setup() {
  Serial.begin(9600); 
  while (!Serial); // waits for the Serial monitor to open before proceeding with the rest of the code
  
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
}

void loop() {
  
//  // -- BLUETOOTH -- //
//
//  // Wait for a BLE central to connect
//  BLEDevice central = BLE.central();
//
//  while(!centralFlag) {
//    central = BLE.central();
////    Serial.println(central);
//    if(central) {
//      centralFlag = true;
//    }
//    else
//    {
//      Serial.println(central);
//    }
//  }
//
//  if (central) {
//    if (isCentralConnected == false) {
//      isCentralConnected = true;
//      Serial.print("Connected to central: ");
//      Serial.println(central.address());
//      digitalWrite(LED_BUILTIN, HIGH); // Turn on LED to indicate connection
//    }
//    
//    // Run this in the loop if connected to bluetooth device
//    if (central.connected()) {
//      // Check if the characteristic was written
//      if (customCharacteristic.written()) {
//       // Get the length of the received data
//        int length = customCharacteristic.valueLength();
//
//        // Read the received data
//        const unsigned char* receivedData = customCharacteristic.value();
//
//        // Create a properly terminated string
//        char receivedString[length + 1]; // +1 for null terminator
//        memcpy(receivedString, receivedData, length);
//        receivedString[length] = '\0'; // Null-terminate the string
//
//        // Print the received data to the Serial Monitor
//        Serial.print("Received data: ");
//        Serial.println(receivedString);
//
//        // Optionally, respond by updating the characteristic's value
//        customCharacteristic.writeValue("Data received");
//      }
//    }
//  }
//    else {
//      isCentralConnected = false;
//      digitalWrite(LED_BUILTIN, LOW); // Turn off LED when disconnected
//      Serial.println("Disconnected from central.");
//    }
//  
//  // -- BLUETOOTH END -- //

  readnEncodeSixSonars(counter);

  counter++;

}


void readnEncodeSixSonars(int sonarIndex) {

  if (sonarIndex % 6 == 0) {
    sonarA.readnEncodeDistance2();
    sonarA.displayDistance();
    Serial.println(sonarA.getEncodedString());
  }

  if (sonarIndex % 6 == 1) {
    sonarB.readnEncodeDistance2();
    sonarB.displayDistance();
    Serial.println(sonarB.getEncodedString());
  }

  if (sonarIndex % 6 == 2) {
    sonarC.readnEncodeDistance2();
    sonarC.displayDistance();
    Serial.println(sonarC.getEncodedString());  
  }

  if (sonarIndex % 6 == 3) {
//    sonarD.readnEncodeDistance2();
//    sonarD.displayDistance();
//    Serial.println(sonarD.getEncodedString());
      Serial.println("Sonar D");
  }

  if (sonarIndex % 6 == 4) {
    sonarE.readnEncodeDistance2();
    sonarE.displayDistance();
    Serial.println(sonarE.getEncodedString());
  }

  if (sonarIndex % 6 == 5) {
    sonarF.readnEncodeDistance2();
    sonarF.displayDistance();
    Serial.println(sonarF.getEncodedString());
  }
}

void setupBluetooth() {
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
}
