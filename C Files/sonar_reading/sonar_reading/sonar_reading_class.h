#ifndef SONAR_READING_H
#define SONAR_READING_H

#include <Arduino.h>

class SonarReading {
  private:
    int trigPin, echoPin;
    long duration, inches, cm;
    char ID;
    String encodedString;
  
    long microsecondsToInches(long microseconds);
    long microsecondsToCentimeters(long microseconds);
    void assignID();
    void stringEncoder();
  
  public:
    // constructor declaration
    SonarReading(int trigPin, int echoPin);
  
    void readnEncodeDistance();
    void displayDistance();
    char getID();
    String getEncodedString();

};

#endif
