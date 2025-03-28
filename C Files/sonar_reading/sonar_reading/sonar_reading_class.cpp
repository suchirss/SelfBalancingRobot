#include "Arduino_BMI270_BMM150.h"
#include "sonar_reading_class.h"
#include "Arduino.h"

// constructor implementation
SonarReading::SonarReading(int trigPin, int echoPin) {
  this->trigPin = trigPin;
  this->echoPin = echoPin;
  
  pinMode(this->trigPin, OUTPUT);
  pinMode(this->echoPin, INPUT);

  assignID();
}  

void SonarReading::assignID() {
  switch (trigPin) {
    case A2: ID = 'A'; break;
    case A6: ID = 'B'; break;
    case A0: ID = 'C'; break;
    case D8: ID = 'D'; break;
    case A4: ID = 'E'; break;
    case D11: ID = 'F'; break;
    default: ID = '?'; // Unknown ID
  }
}

char SonarReading::getID() {
  return ID;
}

void SonarReading::readnEncodeDistance() {

  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  duration = pulseIn(echoPin, HIGH, 100000); // duration will return 0 if  timed out

  if (duration != 0) {
    // convert the time into a distance
//    inches = microsecondsToInches(duration);
    cm = microsecondsToCentimeters(duration);
  }

  stringEncoder();
}

void SonarReading::displayDistance() {
  Serial.print("Sonar "); Serial.print(ID); Serial.print(": ");
//  Serial.print(inches);
//  Serial.print(" in, ");
  Serial.print(cm);
  Serial.println(" cm");
}

void SonarReading::stringEncoder() {
  encodedString = String(ID) + String(cm);
}

String SonarReading::getEncodedString() {
  return encodedString;
}

//long SonarReading::microsecondsToInches(long microseconds) {
//  return microseconds / 74 / 2;
//}

long SonarReading::microsecondsToCentimeters(long microseconds) {
  return microseconds / 29 / 2;
}
