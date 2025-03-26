#include "Arduino_BMI270_BMM150.h"
#include "sonar_reading_class.h"
#include "Arduino.h"

std::map<std::pair<int, int>, String> sonarMap = {
  {{A0, A1}, "A"},
  {{A0, A1}, "A"},
  {{A0, A1}, "A"},
  {{A0, A1}, "A"},
  {{A0, A1}, "A"},
  {{A0, A1}, "A"}
};

// constructor implementation
SonarReading::SonarReading(int trigPin, int echoPin) {
  this->trigPin = trigPin;
  this->echoPin = echoPin;
  
  pinMode(this->trigPin, OUTPUT);
  pinMode(this->echoPin, INPUT);

  sonarID = "unknown"; // default value prior to assignID being called
  assignID();
}  

void SonarReading::assignID() {
  std::pair<int, int> pinPair = {trigPin, echoPin};
  if (sonarMap.find(pinPair) != sonarMap.end()) {
    sonarID = sonarMap[pinPair];
  }
  else {
    sonarID = "unknown";
  }
}

String SonarReading::getID() {
  return sonarID;
}

void SonarReading::readDistance() {

  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  duration = pulseIn(echoPin, HIGH);

  // convert the time into a distance
  inches = microsecondsToInches(duration);
  cm = microsecondsToCentimeters(duration);

  delay(100);
}

void SonarReading::displayDistance() {

  Serial.print("--------------------------- "); Serial.print("ID: "); Serial.print(sonarID); Serial.println(" ---------------------------");
  Serial.print(inches);
  Serial.print(" in, ");
  Serial.print(cm);
  Serial.println(" cm");
}

long SonarReading::microsecondsToInches(long microseconds) {
  return microseconds / 74 / 2;
}

long SonarReading::microsecondsToCentimeters(long microseconds) {
  return microseconds / 29 / 2;
}
