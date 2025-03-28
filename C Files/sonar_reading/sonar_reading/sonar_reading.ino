#include "Arduino_BMI270_BMM150.h"
#include "sonar_reading_class.h"

SonarReading sonarA(A2, A3); // correct
SonarReading sonarB(A6, A7); // correct
SonarReading sonarC(A0, A1); // correct
SonarReading sonarD(D8, D7); // correct
SonarReading sonarE(A4, A5); // correct
SonarReading sonarF(D11, D12); // correct


void setup() {
  Serial.begin(9600); 
}

void loop() {
  sonarA.readDistance();
  sonarA.displayDistance();
  Serial.println(sonarA.encodeString());

  sonarB.readDistance();
  sonarB.displayDistance();
  Serial.println(sonarB.encodeString());

  sonarC.readDistance();
  sonarC.displayDistance();
  Serial.println(sonarC.encodeString());  

  sonarD.readDistance();
  sonarD.displayDistance();
  Serial.println(sonarD.encodeString());

  sonarE.readDistance();
  sonarE.displayDistance();
  Serial.println(sonarE.encodeString());

  sonarF.readDistance();
  sonarF.displayDistance();
  Serial.println(sonarF.encodeString());

}
