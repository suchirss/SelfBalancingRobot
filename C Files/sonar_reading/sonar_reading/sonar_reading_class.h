#ifndef SONAR_READING_H
#define SONAR_READING_H

#include <Arduino.h>
#include <map>

class SonarReading {
  private:
    int trigPin, echoPin;
    long duration, inches, cm;
    String sonarID;
  
    long microsecondsToInches(long microseconds);
    long microsecondsToCentimeters(long microseconds);
    void assignID();
  
  public:
    float distance;

    // constructor declaration
    SonarReading(int trigPin, int echoPin);
  
    void readDistance();
  
    void displayDistance();

    String getID();
};

#endif
