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

  duration = pulseIn(echoPin, HIGH, 25000); // duration will return 0 if  timed out

  if (duration != 0) {
    // convert the time into a distance
//    inches = microsecondsToInches(duration);
    cm = microsecondsToCentimeters(duration);
  }

  stringEncoder();
}

int SonarReading::readnEncodeDistance2() {

  unsigned long timeout = 30000; // Timeout duration in microseconds
  // Send a 10-microsecond pulse to the trigPin to start the sonar measurement
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2); // Short delay to ensure the trigger pin is clean
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10); // Send the pulse for 10 microseconds
  digitalWrite(trigPin, LOW);

  // Measure the pulse duration on the echoPin using micros()
  unsigned long startTime = micros();  // Record the start time
  while (digitalRead(echoPin) == LOW) {
    if (micros() - startTime > timeout) {
      cm = -1; // resets cm
      stringEncoder();
      return -1;
    }
  }

  unsigned long endTime = micros();  // Record the end time
  while (digitalRead(echoPin) == HIGH) {
    if (micros() - startTime > timeout) {
      cm = -1; // resets cm
      stringEncoder();
      return -1;
    }
    endTime = micros(); // Keep updating the end time until echo goes LOW
  }

  // Calculate the duration of the pulse (echo time in microseconds)
  duration = endTime - startTime;

  // Convert the duration to distance in centimeters
  cm = microsecondsToCentimeters(duration);

  stringEncoder();
  return 0;
}



void SonarReading::displayDistance() {
  Serial.print("Sonar "); Serial.print(ID); Serial.print(": ");
//  Serial.print(inches);
//  Serial.print(" in, ");
  Serial.print(cm);
  Serial.println(" cm");
}

void SonarReading::stringEncoder() {
  Serial.print("cm: "); Serial.println(cm);
  if(cm != -1) {
    encodedString = "#" + String(ID) + String(cm);
  } else {
      encodedString = "";  // Clear the string when cm is -1
  }
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
