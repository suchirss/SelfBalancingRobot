#include "Arduino_BMI270_BMM150.h"
#include "sonar_reading_class.h"

SonarReading sonar1(A0, A1);

void setup() {
  Serial.begin(9600); 
}

void loop() {
  sonar1.readDistance();
  sonar1.displayDistance();
}
