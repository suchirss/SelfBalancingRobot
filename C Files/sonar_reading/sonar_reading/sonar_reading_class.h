#ifndef SONAR_READING_H
#define SONAR_READING_H

#include <Arduino.h>
#include <map>

class SonarReading {
  private:
    int trigPin, echoPin;
    long duration, inches, cm;
    char ID;
  
    long microsecondsToInches(long microseconds);
    long microsecondsToCentimeters(long microseconds);
    void assignID();
  
  public:
    // constructor declaration
    SonarReading(int trigPin, int echoPin);
  
    void readDistance();
    void displayDistance();
    char getID();
    String encodeString();

};

#endif
