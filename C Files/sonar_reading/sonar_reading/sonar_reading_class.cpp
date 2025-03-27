#include "Arduino_BMI270_BMM150.h"
#include "sonar_reading_class.h"
#include "Arduino.h"

//std::map<std::pair<int, int>, String> sonarMap = {
//  {{A2, A3}, "A"},
//  {{D11, D12}, "B"},
//  {{A6, A7}, "C"},
//  {{D8, D7}, "D"},
//  {{A0, A1}, "E"},
//  {{A4, A5}, "F"}
//};

// constructor implementation
SonarReading::SonarReading(int trigPin, int echoPin) {
  this->trigPin = trigPin;
  this->echoPin = echoPin;
  
  pinMode(this->trigPin, OUTPUT);
  pinMode(this->echoPin, INPUT);

//  sonarID = "unknown"; // default value prior to assignID being called
//  assignID();
}  

//void SonarReading::assignID() {
//  std::pair<int, int> pinPair = {trigPin, echoPin};
//
//  Serial.print("Assigning ID for pinPair: ");
//  Serial.print(trigPin); Serial.print(", "); Serial.println(echoPin);
//  
//  if (sonarMap.find(pinPair) != sonarMap.end()) {
//    sonarID = sonarMap[pinPair];
//    Serial.print("Assigned ID: ");
//    Serial.println(sonarID);
//  }
//  else {
//    sonarID = "unknown";
//    Serial.println("No matching ID found");
//  }
//}
//
//String SonarReading::getID() {
//  return sonarID;
//}

void SonarReading::readDistance() {

  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  duration = pulseIn(echoPin, HIGH, 30000);

  // convert the time into a distance
  inches = microsecondsToInches(duration);
  cm = microsecondsToCentimeters(duration);
}

void SonarReading::displayDistance() {

//  Serial.print("--------------------------- "); Serial.print("ID: "); Serial.print(sonarID); Serial.println(" ---------------------------");
  Serial.print(inches);
  Serial.print(" in, ");
  Serial.print(cm);
  Serial.println(" cm");

//  delay(100);
}

long SonarReading::microsecondsToInches(long microseconds) {
  return microseconds / 74 / 2;
}

long SonarReading::microsecondsToCentimeters(long microseconds) {
  return microseconds / 29 / 2;
}
