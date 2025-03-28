#include "Arduino_BMI270_BMM150.h"
#include "sonar_reading_class.h"

SonarReading sonarA(A2, A3); // correct
SonarReading sonarB(A6, A7); // correct
SonarReading sonarC(A0, A1); // correct
SonarReading sonarD(8, 7); 
SonarReading sonarE(A4, A5); // correct
SonarReading sonarF(D11, D12); // correct


void setup() {
  Serial.begin(9600); 
}

void loop() {
  sonarA.readnEncodeDistance();
  sonarA.displayDistance();
  Serial.println(sonarA.getEncodedString());

  sonarB.readnEncodeDistance();
  sonarB.displayDistance();
  Serial.println(sonarB.getEncodedString());

  sonarC.readnEncodeDistance();
  sonarC.displayDistance();
  Serial.println(sonarC.getEncodedString());  
  
  sonarD.readnEncodeDistance();
  sonarD.displayDistance();
  Serial.println(sonarD.getEncodedString());

  sonarE.readnEncodeDistance();
  sonarE.displayDistance();
  Serial.println(sonarE.getEncodedString());

  sonarF.readnEncodeDistance();
  sonarF.displayDistance();
  Serial.println(sonarF.getEncodedString());

}
