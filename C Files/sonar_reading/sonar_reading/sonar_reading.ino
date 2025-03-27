#include "Arduino_BMI270_BMM150.h"
#include "sonar_reading_class.h"

SonarReading sonarA(A2, A3);
SonarReading sonarB(D11, D12);
SonarReading sonarC(A6, A7);
SonarReading sonarD(D8, D7);
SonarReading sonarE(A0, A1);
SonarReading sonarF(A4, A5);

void setup() {
  Serial.begin(2000000); 
}

void loop() {
  sonarA.readDistance();
  sonarA.displayDistance();
  

  sonarB.readDistance();
  sonarB.displayDistance();
  
  sonarC.readDistance();
  sonarC.displayDistance();

  sonarD.readDistance();
  sonarD.displayDistance();

  sonarE.readDistance();
  sonarE.displayDistance();

  sonarF.readDistance();
  sonarF.displayDistance();
}
